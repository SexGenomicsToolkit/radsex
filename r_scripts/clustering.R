source("/home/rferon/work/analyses/radseq_all/r_scripts/utils.R")

coverage_heatmap = function(coverage_file, popmap_file, output_file="none",
                            remove_coverage_1=TRUE, sex_ratio_range=0.25,
                            min_individuals_ratio=0.5, special_SD_system=FALSE,
                            coverage_filtering_range=0.1, max_coverage=150,
                            distance="binary", clustering="ward.D",
                            x_axis = 'individuals', individuals.dendrogram = TRUE, loci.dendrogram = TRUE,
                            males.color="steelblue3", females.color="orange1",
                            palette=c("lightgrey", "royalblue2", "black", "gold2", "red3"),
                            fasta_file_path="none", coverage_file_path="none", species="none") {

    data = load_coverage_file(coverage_file)
    popmap = load_popmap_file(popmap_file)

    data = filter(data, popmap, min_ratio=min_individuals_ratio, remove_coverage_1 = remove_coverage_1, sex_ratio_range=sex_ratio_range,
                  special=special_SD_system, ratio_range=coverage_filtering_range, max_coverage=max_coverage,
                  fasta_file_path = fasta_file_path, coverage_file_path = coverage_file_path, species=species)

    if (dim(data)[1] > 0) {
        if(output_file != "none") { png(output_file, width=2400, height=1200, res=90) }
        plot_heatmap(data, popmap,
                     distance=distance, clustering=clustering,
                     x_axis=x_axis, individuals.dendrogram=individuals.dendrogram, loci.dendrogram=loci.dendrogram,
                     males.color=males.color, females.color=females.color, palette=palette)
        if(output_file != "none") { o = dev.off() }
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



export_to_fasta = function(data, file_path) {

    output_file = file(file_path)
    lines = c()
    for (i in 1:dim(data)[1]) {
        name = ">"
        name = paste(name, data$Name[i], sep="")
        sequence = as.character(data$Sequence[i])
        lines = c(lines, name, sequence)
    }
    writeLines(lines, output_file)
    close(output_file)
}



export_coverage = function(data, file_path) {
    names(data)[1] = "Haplotype"
    write.table(data, file=file_path, quote=FALSE, sep="\t", row.names=FALSE)
}


filter = function(data, popmap, min_ratio=0.5, special=FALSE, ratio_range=0.1, sex_ratio_range = 0.25, max_coverage=150, remove_coverage_1=TRUE,
                  fasta_file_path="none", coverage_file_path="none", species="none") {

    fasta = TRUE
    if (fasta_file_path == "none") { fasta = FALSE }

    depths = data[, 7:dim(data)[2]]
    depths[depths == 0] = NA
    if (remove_coverage_1){ depths[depths==1] = NA }

    # REMOVE LOCI WITH LESS MIN.RATIO MALES AND LESS THAN MIN.RATIO FEMALES
    min_males = floor(sum(popmap$Sex == 'M') * min_ratio)
    min_females = floor(sum(popmap$Sex == 'F') * min_ratio)
    males_col = which(names(depths) %in% popmap$Individual[which(popmap$Sex == 'M')])
    females_col = which(names(depths) %in% popmap$Individual[which(popmap$Sex == 'F')])
    too_few_individuals = which((rowSums(!is.na(depths[, males_col])) < min_males) & (rowSums(!is.na(depths[, females_col])) < min_females) )
    data = data[-too_few_individuals,]
    depths = depths[-too_few_individuals,]

    # IF A VALUE IS > MAX.COVERAGE PERCENTILE, REPLACE WITH MAX.COVERAGE PERCENTILE
    depths[depths > max_coverage] = max_coverage

    if (!special) {
        # Filter XY alleles
        males_means = rowMeans(depths[, males_col], na.rm = TRUE)
        females_means = rowMeans(depths[, females_col], na.rm = TRUE)
        males_means[males_means=='NaN'] = 0
        females_means[females_means=='NaN'] = 0

        # Sequences in 50% more males than females
        n_males = rowSums(!is.na(depths[, males_col]))
        n_females = rowSums(!is.na(depths[, females_col]))
        sex_ratio = (n_males / (n_males + n_females))
        to_keep = which(sex_ratio > 1-sex_ratio_range | sex_ratio < sex_ratio_range)

        # Sequences with coverage bias
        unbiased_value = 0.5
        ratio = (males_means) / (males_means + females_means)
        to_keep = union(to_keep, which(ratio > unbiased_value + ratio_range | ratio < unbiased_value - ratio_range))

        # Filtering
        depths = depths[to_keep,]
        data = data[to_keep,]
    }

    depths[is.na(depths)] = 0

    data = data.frame(Origin=rep("", dim(data)[1]), Males=rowSums(depths[, males_col] > 0), Females=rowSums(depths[, females_col] > 0),
                      Other_allele=rep("", dim(data)[1]), data[,1:6], depths)
    individual_names = names(depths)
    data$Origin = as.character(data$Origin)
    data$Haplotype_ID = as.character(data$Haplotype_ID)
    data$Stack_ID = as.character(data$Stack_ID)
    data$Other_allele = as.character(data$Other_allele)

    for (i in 1:dim(data)[1]) {
        data$Stack_ID[i] = strsplit(as.character(data$Stack_ID[i]), "_")[[1]][1]
        if (data$Haplotype_ID[i] == data$Stack_ID[i]) {
            data$Origin[i] = "Heatmap"
        } else {
            data$Origin[i] = "Blasting"
        }
    }

    heatmap = subset(data, data$Origin == 'Heatmap')
    blasting = subset(data, data$Origin == 'Blasting' & !(data$Haplotype_ID %in% heatmap$Stack_ID))
    blasting$Other_allele = blasting$Stack_ID

    # When a haplotype was rescued from multiple haplotypes, only keep best matching scores
    to_remove = c()
    haplotypes = unique(blasting$Haplotype_ID)
    for (i in 1:length(haplotypes)) {
        temp = max(blasting$Matches[blasting$Haplotype_ID == haplotypes[i]])
        to_remove = c(to_remove, which(blasting$Haplotype_ID == haplotypes[i] & blasting$Matches < temp))
        max = which(blasting$Haplotype_ID == haplotypes[i] & blasting$Matches == temp)
        if (length(max > 1)) {
            to_remove = c(to_remove, max[-1])
            blasting$Other_allele[max[1]] = paste("Complementary:", paste(blasting$Stack_ID[max], collapse=","), sep="")
        }
    }
    if (length(to_remove) > 0) {blasting = blasting[-to_remove, ]}

    for (i in 1:dim(heatmap)[1]) {
        candidates = which(blasting$Stack_ID == heatmap$Haplotype_ID[i])
        if (length(candidates) == 0) {
            heatmap$Other_allele[i] = "Specific"
        } else {
            heatmap$Other_allele[i] = paste("Complementary:", paste(blasting$Haplotype_ID[candidates], collapse=","), sep="")
        }
    }

    combined = rbind(heatmap, blasting)
    new_ids = paste(species, "_", combined$Haplotype_ID, "_", combined$Males, "M_", combined$Females, "F_",
                          combined$Origin, "_", combined$Other_allele, sep="")
    combined = data.frame(Name=new_ids, combined)
    names(combined)[12:dim(combined)[2]] = individual_names

    export_to_fasta(subset(combined, select=c("Name", "Sequence")), fasta_file_path)
    export_coverage(combined[,c(1, 12:dim(combined)[2])], coverage_file_path)


    return(combined[, c(7, 12:dim(combined)[2])])
}


convert_to_presence_data = function(data, read.length=94) {

    data = subset(data, data$Matches == read.length)
    depths = data[, 7:dim(data)[2]]
    depths[depths>0] = 1

    new_data = data.frame(data[,1], depths)
    names(new_data) = c("Haplotype_ID", names(data[, 7:dim(data)[2]]))

    return(new_data)
}


plot_heatmap = function(data, popmap=data.frame(), x_axis='individuals',
                        distance="binary", clustering="ward.D",
                        individuals.dendrogram=TRUE, loci.dendrogram=TRUE,
                        males.color="steelblue3", females.color="orange1",
                        palette=c("lightgrey", "royalblue2", "black", "gold2", "red3"),
                        absence.color="cornsilk", presence.color="skyblue3") {

    x = as.matrix(data[,-1], rownames.force = TRUE)
    rownames(x) =  data$Haplotype_ID

    if (dim(data)[1] > 200) {
        tile_separation_size = 0.05
    } else {
        tile_separation_size = 0.1
    }

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

    melted = suppressMessages(melt(data, id.vars = c("Haplotype_ID"), variable.name = "Individuals", value.name = "Values"))
    melted$Haplotype_ID = factor(melted$Haplotype_ID, levels = data$Haplotype_ID)
    melted$Individuals = factor(as.character(melted$Individuals), levels = as.character(names(data)[-1]))

    if (length(unique(melted$Values)) == 2) {
        melted$Values = factor(melted$Values, levels = unique(melted$Values))
    } else {
        melted$Values = as.numeric(melted$Values)
    }

    all_values = melted$Values
    all_values[all_values==0] = NA
    stats = summary(all_values, na.rm=TRUE)

    if (length(popmap) > 0) {
        sexes = c(popmap$Sex)
        names(sexes) = c(popmap$Individual)
    }

    sex_palette = c("M"=males.color, "F"=females.color)

    if (x_axis == "individuals") {

        g = ggplot(melted, aes(x=Individuals, y=Haplotype_ID, fill=Values)) +
            geom_tile(color="grey60", size=tile_separation_size) +
            theme(axis.text.x = element_text(angle=60, hjust = 1)) +
            ylab("Loci") + xlab("Individuals")

        if (length(unique(all_values)) != 2) {
            g = g + scale_fill_gradientn(name = "Coverage", colours = palette, values = c(0, 0.001, stats[3]/stats[6], stats[5]/stats[6], 1))
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
            geom_tile(color="grey60", size=tile_separation_size) +
            theme(axis.text.x = element_text(angle=60, hjust = 1)) +
            xlab("Loci") + ylab("Individuals")

        if (length(unique(all_values)) != 2) {
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


plot_clustering_clean = function(data, popmap=data.frame(),
                        distance="binary", clustering="ward.D",
                        males.color="steelblue3", females.color="orange1",
                        palette=c("white", "royalblue2", "black", "gold2", "red3")) {

    x = as.matrix(data[,-1], rownames.force = TRUE)
    rownames(x) =  data$Haplotype_ID

    tile_separation_size = 0.02

    individuals.d = dist(t(x), method=distance)
    individuals.clusters = hclust(individuals.d, method=clustering)
    ordered = individuals.clusters$labels[individuals.clusters$order]
    temp = c(1)
    for (i in 1:length(ordered)) {
        temp = c(temp, which(names(data) == ordered[i]))
    }
    data = data[,temp]

    loci.d = dist(x, method=distance)
    loci.clusters = hclust(loci.d, method=clustering)
    ordered = loci.clusters$labels[loci.clusters$order]
    temp = c()
    for (i in 1:length(ordered)) {
        temp = c(temp, which(data$Haplotype_ID == ordered[i]))
    }
    data = data[temp,]

    melted = suppressMessages(melt(data, id.vars = c("Haplotype_ID"), variable.name = "Individuals", value.name = "Values"))
    melted$Haplotype_ID = factor(melted$Haplotype_ID, levels = data$Haplotype_ID)
    melted$Individuals = factor(as.character(melted$Individuals), levels = as.character(names(data)[-1]))
    melted$Values = as.numeric(melted$Values)

    stats = summary(replace(melted$Values, which(melted$Values==0), NA), na.rm=TRUE)

    sexes = c(popmap$Sex)
    names(sexes) = c(popmap$Individual)
    sex_palette = c("M"=males.color, "F"=females.color)

    g = ggplot(melted, aes(x=Individuals, y=Haplotype_ID, fill=Values)) +
        geom_tile(color="grey30", size=tile_separation_size) +
        theme_bw() +
        theme(axis.text.x = element_text(angle=90, vjust=0.5, colour=sex_palette[sexes[names(data)[-1]]], size=10),
              axis.text.y = element_blank(), axis.ticks.y = element_blank(), axis.title.y = element_blank(),
              axis.title.x = element_blank(), plot.margin = margin(15, 25, 15, 15),
              panel.border = element_rect(size=0.75, color="black"),
              legend.position=c(0.85, 0.15), legend.margin = margin(0, 0, 0, 0),
              legend.title = element_text(size=14, face="bold"), legend.text = element_text(size=11),
              legend.key.height = unit(0.06, "npc"), legend.key.width = unit(0.04, "npc")) +
        scale_fill_gradientn(name="Coverage", colours = palette, values = c(0, 0.001, stats[4]/stats[6], stats[5]/stats[6], 1))

    a = ggplotGrob(g)

    h = suppressMessages(ggdendrogram(individuals.clusters, labels = FALSE, leaf_labels = FALSE, theme_dendro = TRUE, rotate = FALSE) +
                         theme(plot.margin = unit(c(0.1, 0, 0, 0), 'cm'), axis.text.x = element_blank(),
                         axis.text.y = element_blank(), axis.title.x=element_blank()) +
                         scale_y_reverse(expand = c(0,0.5)) + scale_x_continuous(expand=c(0,0)))

    a = gtable_add_rows(a, unit(0.06, "npc"), pos=8)
    a = gtable_add_grob(a, ggplotGrob(h), t = 8, l=4, b=9, r=5)
    # grid.newpage()
    # grid.draw(a)
    return(a)
}
