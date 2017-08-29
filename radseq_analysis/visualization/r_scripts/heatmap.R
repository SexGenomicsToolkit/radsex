#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

# test if there is at least one argument: if not, return an error
if (length(args) != 3){
    stop("Usage: R frequencies.R input_file.tsv output_file.png species_name")
}

suppressMessages(require(readr))
suppressMessages(require(ggplot2))

input_file_path = args[1]
output_file_path = args[2]
species_name = args[3]

g_legend<-function(a.gplot){
    tmp <- ggplot_gtable(ggplot_build(a.gplot))
    leg <- which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
    legend <- tmp$grobs[[leg]]
    return(legend)}

sex_heatmap = function(file_path, species_name="none"){

    data = suppressMessages(read_delim(file_path, "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE))

    bins = c(0, 1, 2, 5, 25, 100, 1000)
    bin_labels = c('0', '1', '2-4', '5-24', '25-99', '100-999', '>1000')
    get_palette = colorRampPalette(c("white", "navyblue"))
    palette = get_palette(length(bins))

    males = rep(seq(0, dim(data)[2] - 1), dim(data)[1])
    females = c()
    for (i in 1:dim(data)[1]){
        females = c(females, rep(i-1, dim(data)[2]))
    }
    loci = c()
    for (i in 1:dim(data)[1]){
        loci = c(loci, as.vector(unlist(data[i,])))
    }

    d = data.frame('Males' = males, 'Females' = females, 'Loci' = loci)
    d$Loci = d$Loci + 1

    group = c()
    for (i in 1:dim(d)[1]) {
        group = c(group, bins[tail(which(d$Loci[i]>=bins), n=1)])
    }
    d$Bin = factor(group)

    title = ""
    if (species_name != "none") {
        title = paste(title, species_name, sep="")
    }

    g = ggplot(d, aes(x=Males, y=Females)) +
        geom_tile(aes(fill=Bin), color = "grey") +
        ggtitle(title) + theme(plot.title = element_text(hjust = 0.5)) +
        scale_fill_manual(name="Number of\nhaplotypes", values = palette, labels = bin_labels) +
        scale_x_continuous(breaks = seq(0, max(d$Males), 5), minor_breaks = seq(0, max(d$Males), 1)) +
        scale_y_continuous(breaks = seq(0, max(d$Females), 5), minor_breaks = seq(0, max(d$Females), 1))

    return(g)
}

heatmap = sex_heatmap(input_file_path, species_name)

n_males = max(ggplot_build(heatmap)$data[[1]]$x)
n_females = max(ggplot_build(heatmap)$data[[1]]$y)
ratio = n_females / n_males

width_c = 1200

png(output_file_path, width=width_c, height=width_c*ratio, res=130)
print(heatmap)
x=dev.off()

