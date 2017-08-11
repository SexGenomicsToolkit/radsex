library(readr)
library(ggplot2)
library(grid)
library(gridExtra)

g_legend<-function(a.gplot){
    tmp <- ggplot_gtable(ggplot_build(a.gplot))
    leg <- which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
    legend <- tmp$grobs[[leg]]
    return(legend)}

sex_heatmap = function(file_path, m_value){

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

    g = ggplot(d, aes(x=Males, y=Females)) +
        geom_tile(aes(fill=Bin), color = "grey") +
        ggtitle(paste("m=", m_value, sep="")) + theme(plot.title = element_text(hjust = 0.5)) +
        scale_fill_manual(name="Number of\nhaplotypes", values = palette, labels = bin_labels) +
        scale_x_continuous(breaks = seq(0, max(d$Males), 5), minor_breaks = seq(0, max(d$Males), 1)) +
        scale_y_continuous(breaks = seq(0, max(d$Females), 5), minor_breaks = seq(0, max(d$Females), 1))

    return(g)
}

folder = "/home/rferon/work/analyses/radseq_all/other_species/astatotilapia/"

file_2 = paste(folder, "m_2.tsv", sep="")
if (file.exists(file_2)){
    plot_2 = sex_heatmap(file_2, 2)
} else {
    print(file_2)
}

file_3 = paste(folder, "m_3.tsv", sep="")
if (file.exists(file_3)){
    plot_3 = sex_heatmap(file_3, 3)
} else {
    print(file_3)
}

file_5 = paste(folder, "m_5.tsv", sep="")
if (file.exists(file_5)) {
    plot_5 = sex_heatmap(file_5, 5)
} else {
    print(file_5)
}

file_10 = paste(folder, "m_10.tsv", sep="")
if (file.exists(file_10)){
    plot_10 = sex_heatmap(file_10, 10)
} else {
    print(file_10)
}

n_males = max(ggplot_build(plot_5)$data[[1]]$x)
n_females = max(ggplot_build(plot_5)$data[[1]]$y)
ratio = n_females / n_males
n_plots = 0
if (file.exists(file_2)) {n_plots = n_plots + 1}
if (file.exists(file_3)) {n_plots = n_plots + 1}
if (file.exists(file_5)) {n_plots = n_plots + 1}
if (file.exists(file_10)) {n_plots = n_plots + 1}

width_c = 1600

png(paste(folder, "heatmap.png", sep = ""), width=width_c, height=width_c*ratio, res=100)

species_name = "Astatotilapia"

grid.arrange(plot_2 + theme(legend.position="none"), plot_3 + theme(legend.position="none"), plot_5 + theme(legend.position="none"), plot_10 + theme(legend.position="none"),
ncol=2, nrow=2, top=species_name, right=g_legend(plot_2), widths = c(n_males, n_males), heights = c(n_females, n_females))

dev.off()

