library(readr)
library(ggplot2)
library(reshape2)
library(ggdendro)
library(grid)
library(gtable)


load_popmap_file = function(popmap_file) {

    popmap = suppressMessages(read_delim(popmap_file, "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE))
    names(popmap) = c('Individual', 'Sex')
    return(popmap)
}


load_coverage_file = function(coverage_file) {

    data = suppressMessages(read_delim(coverage_file, "\t", escape_double = FALSE, trim_ws = TRUE))

    data$Stack_ID = factor(data$Stack_ID)
    data$Haplotype_ID = factor(data$Haplotype_ID)

    return(data)
}


extract_XY_alleles = function(data, popmap) {

    depths = data[, 2:dim(data)[2]]

    n_males = sum(popmap$Sex == 'M')
    n_females = sum(popmap$Sex == 'M')

    males_col = which(names(depths) %in% popmap$Individual[which(popmap$Sex == "M")])
    females_col = which(names(depths) %in% popmap$Individual[which(popmap$Sex == "F")])

    males = depths[, males_col]
    females = depths[, females_col]

    males_means = rowMeans(males, na.rm = TRUE)
    females_means = rowMeans(females, na.rm = TRUE)

    ratio = (males_means) / (males_means + females_means)
    data = data[which(ratio > 0.9 | ratio < 0.1 | (ratio > 0.3 & ratio < 0.36) | (ratio > 0.63 & ratio < 0.7)), ]

    return(data)
}


filter_coverage = function(data, popmap, min_ratio=0.5) {

    ### FILTERING ###

    depths = data[, 7:dim(data)[2]]
    depths[depths == 0] = NA

    n_males = sum(popmap$Sex == 'M')
    n_females = sum(popmap$Sex == 'M')
    min_males = n_males * min_ratio
    min_females = n_females * min_ratio

    males_col = which(names(depths) %in% popmap$Individual[which(popmap$Sex == "M")])
    females_col = which(names(depths) %in% popmap$Individual[which(popmap$Sex == "F")])

    too_few = which(apply(depths[, males_col], 1, function(x) sum(is.na(x))) > min_males & apply(depths[, females_col], 1, function(x) sum(is.na(x))) > min_females)

    # REMOVE LOCI WITH LESS THAN 50% MALES AND LESS THAN 50% FEMALES
    data = data[-too_few,]
    depths = depths[-too_few,]

    mean_coverage = rowMeans(depths, na.rm = TRUE)
    max_coverage = round(quantile(mean_coverage, c(.95))[1])

    to_remove = which(mean_coverage > max_coverage)

    # REMOVE LOCI WITH AVERAGE COVERAGE IN 95th PERCENTILE
    data = data[-to_remove,]

    data = data[, c(2, 7:dim(data)[2])]
    data = unique(data)

    if (dim(data)[1] == 0) {
        stop()
    }

    temp = suppressMessages(melt(data))
    temp[temp == 0] = NA
    max_coverage = round(quantile(temp$value, c(.99), na.rm = TRUE)[1])
    # IF A VALUE IS > 99th PERCENTILE, REPLACE WITH 99th PERCENTILE
    suppressWarnings(data[data>max_coverage] <- max_coverage)

    return(data)
}


convert_to_presence_data = function(data, read.length=94) {

    data = subset(data, data$Matches == read.length)
    depths = data[, 7:dim(data)[2]]
    depths[depths>0] = 1

    data = data.frame(data[,1], depths)
    names(data)[1] = "Haplotype_ID"

    return(data)
}


plot_heatmap = function(data, popmap=data.frame(), x_axis='individuals',
                        distance="binary", clustering="ward.D",
                        individuals.dendrogram=TRUE, loci.dendrogram=TRUE,
                        males.color="steelblue3", females.color="orange1",
                        palette=c("lightgrey", "royalblue2", "black", "gold2", "red3"),
                        absence.color="cornsilk", presence.color="skyblue3") {

    x = as.matrix(data[,-1], rownames.force = TRUE)
    rownames(x) =  data$Haplotype_ID

    individuals.d = dist(t(x), method=distance)
    individuals.clusters = hclust(individuals.d, method=clustering)
    ordered = individuals.clusters$labels[individuals.clusters$order]
    temp = c(1)
    for (i in 1:length(ordered)) {
        temp = c(temp, which(names(data) == ordered[i]))
    }
    individuals.order = temp

    loci.d = dist(x, method=distance)
    loci.clusters = hclust(loci.d, method=clustering)
    ordered = loci.clusters$labels[loci.clusters$order]
    temp = c()
    for (i in 1:length(ordered)) {
        temp = c(temp, which(data$Haplotype_ID == ordered[i]))
    }
    loci.order = temp

    data = data[,individuals.order]
    data = data[loci.order,]

    melted = suppressMessages(melt(data, variable.name = "Individuals", value.name = "Values"))
    melted$Haplotype_ID = factor(melted$Haplotype_ID, levels = data$Haplotype_ID)
    melted$Individuals = factor(melted$Individuals, levels = names(data)[-1])

    if (length(unique(melted$Values)) == 2) {
        melted$Values = factor(as.character(melted$Values), levels = as.character(unique(melted$Values)))
    }

    if (length(popmap) > 0) {
        sexes = c(popmap$Sex)
        names(sexes) = c(popmap$Individual)
    }

    sex_palette = c("M"=males.color, "F"=females.color)

    if (x_axis == "individuals") {

        g = ggplot(melted, aes(x=Individuals, y=Haplotype_ID, fill=Values)) +
            geom_tile(color="grey") +
            theme(axis.text.x = element_text(angle=60, hjust = 1)) +
            ylab("Loci") + xlab("Individuals")

        if (length(unique(melted$Values)) != 2) {
            temp = melted
            temp$Values[temp$Values==0] = NA
            stats = summary(temp$Values, na.rm=TRUE)
            g = g + scale_fill_gradientn(name = "Coverage", colours = palette, values = c(0, 0.001, stats[4]/stats[6], stats[5]/stats[6], 1))
        } else {
            g = g + scale_fill_manual(breaks = c("0", "1"), labels = c("Absent", "Present"), values = c("0"=absence.color, "1"=presence.color)) +
                theme(legend.title=element_blank())
        }

        if (length(popmap) > 0) {
            g = g + theme(axis.text.x = element_text(colour=sex_palette[sexes[names(data)[-1]]]))
        }

        a = ggplotGrob(g)
        loci_start = 4

        if (individuals.dendrogram == TRUE) {

            h = suppressMessages(ggdendrogram(individuals.clusters, labels = FALSE, leaf_labels = FALSE, theme_dendro = TRUE, rotate = FALSE) +
                                     theme(plot.margin = unit(c(0.1, 0, 0, 0), 'cm'), axis.text.x = element_blank(), axis.text.y = element_blank(), axis.title.x=element_blank()) +
                                     scale_y_continuous(expand = c(0,0.5)) + scale_x_continuous(expand=c(0,0.5)))
            b = ggplotGrob(h)
            a = gtable_add_rows(a, unit(1.1, "cm"), pos=3)
            a = gtable_add_grob(a, b, t = 3, l=4, b=5, r=5)
            loci_start = loci_start + 1
        }

        if (loci.dendrogram == TRUE) {

            i = suppressMessages(ggdendrogram(loci.clusters, labels = FALSE, leaf_labels = FALSE, theme_dendro = TRUE, rotate = FALSE) +
                                     theme(plot.margin = unit(c(0.1, 0, 0, 0), 'cm'), axis.text.x = element_blank(), axis.text.y = element_blank(), axis.title.x=element_blank()) +
                                     coord_flip() + scale_y_continuous(expand = c(0,0.5)) + scale_x_continuous(expand=c(0,0)))
            c = ggplotGrob(i)
            a = gtable_add_cols(a, unit(1.1, "cm"), pos=5)
            a = gtable_add_grob(a, c, t = loci_start, l=5, b=loci_start + 2, r=6)
        }

        grid.newpage()
        grid.draw(a)

    } else if (x_axis == "loci") {

        g = ggplot(melted, aes(x=Haplotype_ID, y=Individuals, fill=Values)) +
            geom_tile(color="grey") +
            theme(axis.text.x = element_text(angle=60, hjust = 1)) +
            xlab("Loci") + ylab("Individuals")

        if (length(unique(melted$Values)) != 2) {
            temp = melted
            temp$Values[temp$Values==0] = NA
            stats = summary(temp$Values, na.rm=TRUE)
            g = g + scale_fill_gradientn(name="Coverage", colours = palette, values = c(0, 0.001, stats[4]/stats[6], stats[5]/stats[6], 1))
        } else {
            g = g + scale_fill_manual(breaks = c("0", "1"), labels = c("Absent", "Present"), values = c("0"=absence.color, "1"=presence.color)) +
                theme(legend.title=element_blank())
        }

        if (length(popmap) > 0) {
            g = g + theme(axis.text.y = element_text(colour=sex_palette[sexes[names(data)[-1]]]))
        }

        a = ggplotGrob(g)
        loci_start = 4

        if (loci.dendrogram == TRUE) {

            h = suppressMessages(ggdendrogram(individuals.clusters, labels = FALSE, leaf_labels = FALSE, theme_dendro = TRUE, rotate = FALSE) +
                                     theme(plot.margin = unit(c(0.1, 0, 0, 0), 'cm'), axis.text.x = element_blank(), axis.text.y = element_blank(), axis.title.x=element_blank()) +
                                     scale_y_continuous(expand = c(0,0.5)) + scale_x_continuous(expand=c(0,0.5)))
            b = ggplotGrob(h)
            a = gtable_add_rows(a, unit(1.1, "cm"), pos=3)
            a = gtable_add_grob(a, b, t = 3, l=4, b=5, r=5)
            loci_start = loci_start + 1
        }

        if (individuals.dendrogram == TRUE) {

            i = suppressMessages(ggdendrogram(loci.clusters, labels = FALSE, leaf_labels = FALSE, theme_dendro = TRUE, rotate = FALSE) +
                                     theme(plot.margin = unit(c(0.1, 0, 0, 0), 'cm'), axis.text.x = element_blank(), axis.text.y = element_blank(), axis.title.x=element_blank()) +
                                     coord_flip() + scale_y_continuous(expand = c(0,0.5)) + scale_x_continuous(expand=c(0,0)))
            c = ggplotGrob(i)
            a = gtable_add_cols(a, unit(1.1, "cm"), pos=5)
            a = gtable_add_grob(a, c, t = loci_start, l=5, b=loci_start + 2, r=6)
        }

        grid.newpage()
        grid.draw(a)
    }
}


coverage_heatmap = function(coverage_file, popmap_file, min_individuals_ratio=0.5,
                            distance="binary", clustering="ward.D",
                            x_axis = 'individuals', individuals.dendrogram = TRUE, loci.dendrogram = TRUE,
                            males.color="steelblue3", females.color="orange1",
                            palette=c("lightgrey", "royalblue2", "black", "gold2", "red3")) {

    data = load_coverage_file(coverage_file)
    popmap = load_popmap_file(popmap_file)

    data = filter_coverage(data, popmap, min_ratio=min_individuals_ratio)
    data = extract_XY_alleles(data, popmap)

    if (dim(data)[1] > 0) {
        plot_heatmap(data, popmap,
                     distance=distance, clustering=clustering,
                     x_axis=x_axis, individuals.dendrogram=individuals.dendrogram, loci.dendrogram=loci.dendrogram,
                     males.color=males.color, females.color=females.color, palette=palette)
    }
}


presence_heatmap = function(coverage_file, popmap_file, read.length=94,
                            distance="binary", clustering="ward.D",
                            x_axis='individuals', individuals.dendrogram=TRUE, loci.dendrogram=TRUE,
                            males.color="steelblue3", females.color="orange1",
                            absence.color="cornsilk", presence.color="skyblue3") {

    data = load_coverage_file(coverage_file)
    popmap = load_popmap_file(popmap_file)

    data = convert_to_presence_data(data, read.length=read.length)

    if (dim(data)[1] > 0) {
        plot_heatmap(data, popmap,
                     distance=distance, clustering=clustering,
                     x_axis=x_axis, individuals.dendrogram=individuals.dendrogram, loci.dendrogram=loci.dendrogram,
                     males.color=males.color, females.color=females.color,
                     absence.color=absence.color, presence.color=presence.color)
    }
}
