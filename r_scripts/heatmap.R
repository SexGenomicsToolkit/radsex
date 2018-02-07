source("/home/rferon/work/analyses/radseq_all/r_scripts/utils.R")


heatmap = function(haplotypes_matrix_path, output_file, title="none", species="none", m_value = "none", show.significance=TRUE, width_c=1200) {

    if (title == "none" & species == "none") {title="Haplotypes distribution"}
    title = get_title(species=species, m_value=m_value, base=title)

    if (file.exists(haplotypes_matrix_path)){
        haplotypes_matrix = load_haplotypes_matrix(haplotypes_matrix_path)
        g = generate_heatmap(haplotypes_matrix, show.significance = show.significance, title = title)
    } else {
        print(paste("Haplotypes matrix file", haplotypes_matrix_path, "does not exist.", sep=" "))
        g = "none"
    }

    n_males = max(ggplot_build(g)$data[[1]]$x)
    n_females = max(ggplot_build(g)$data[[1]]$y)
    ratio = n_females / n_males

    png(paste(output_file, ".png", sep=""), width=1.15*width_c, height=width_c*ratio, res=200)
    grid.arrange(g + theme(legend.position = "none"), right = g_legend(g))
    o=dev.off()

    svg(paste(output_file, ".svg", sep=""), width=1.15*width_c/200, height=ratio*width_c/200)
    grid.arrange(g + theme(legend.position = "none"), right = g_legend(g))
    o=dev.off()
}

separate_heatmaps = function(input_folder, output_folder="none", title="none", species="none", show.significance = TRUE) {

    input_folder = paste(sub("/$", "", input_folder))
    output_folder = paste(sub("/$", "", output_folder))

    data = parse_folder(input_folder, show.significance=show.significance, title=title, species=species)

    separate_heatmaps_plot(data, output_folder, species)

}



aggregate_heatmaps = function(input_folder, output_file="none", title = "none", species="none", show.significance = TRUE) {

    input_folder = paste(sub("/$", "", input_folder))

    data = parse_folder(input_folder, show.significance=show.significance, title="", species="none")

    aggregate_heatmaps_plot(data, output_file, title=title, species=species)

}



parse_folder = function(folder, show.significance=TRUE, title="none", species="none") {

    output = list()

    plot_m_2 = single_heatmap(folder, 2, show.significance = show.significance, title=title, species=species)
    plot_m_3 = single_heatmap(folder, 3, show.significance = show.significance, title=title, species=species)
    plot_m_5 = single_heatmap(folder, 5, show.significance = show.significance, title=title, species=species)
    plot_m_10 = single_heatmap(folder, 10, show.significance = show.significance, title=title, species=species)

    if(is.ggplot(plot_m_2)) {
        n_males = max(ggplot_build(plot_m_2)$data[[1]]$x)
        n_females = max(ggplot_build(plot_m_2)$data[[1]]$y)
    } else if (is.ggplot(plot_m_3)){
        n_males = max(ggplot_build(plot_m_3)$data[[1]]$x)
        n_females = max(ggplot_build(plot_m_3)$data[[1]]$y)
    } else if (is.ggplot(plot_m_5)){
        n_males = max(ggplot_build(plot_m_5)$data[[1]]$x)
        n_females = max(ggplot_build(plot_m_5)$data[[1]]$y)
    } else if (is.ggplot(plot_m_10)){
        n_males = max(ggplot_build(plot_m_10)$data[[1]]$x)
        n_females = max(ggplot_build(plot_m_10)$data[[1]]$y)
    }

    ratio = n_females / n_males
    n_plots = 0
    if (is.ggplot(plot_m_2)) {n_plots = n_plots + 1}
    if (is.ggplot(plot_m_3)) {n_plots = n_plots + 1}
    if (is.ggplot(plot_m_5)) {n_plots = n_plots + 1}
    if (is.ggplot(plot_m_10)) {n_plots = n_plots + 1}

    output$plot_m_2 = plot_m_2
    output$plot_m_3 = plot_m_3
    output$plot_m_5 = plot_m_5
    output$plot_m_10 = plot_m_10
    output$n_males = n_males
    output$n_females = n_females
    output$ratio = ratio
    output$n_plots = n_plots

    return(output)
}



single_heatmap = function(folder, m_value, show.significance=TRUE, title="none", species="none") {

    haplotypes_matrix_path = paste(folder, paste("m_", m_value, ".tsv", sep=""), sep="/")

    if (title == "none" & species == "none") {title="Haplotypes distribution"}
    title = get_title(species=species, m_value=m_value, base=title)

    if (file.exists(haplotypes_matrix_path)){
        haplotypes_matrix = load_haplotypes_matrix(haplotypes_matrix_path)
        g = generate_heatmap(haplotypes_matrix, show.significance = show.significance, title = title)
    } else {
        print(paste("Haplotypes matrix file", haplotypes_matrix_path, "does not exist.", sep=" "))
        g = "none"
    }

    return(g)
}



separate_heatmaps_plot = function(data, output_folder, species = "none") {

    if (is.ggplot(data$plot_m_2)) {
        if (output_folder != "none") { png(paste(output_folder, "heatmap_m_2.png",  sep="/"), width=800, height=800*data$ratio, res=100) }
        print(data$plot_m_2)
        if (output_folder != "none") { o=dev.off() }
    }

    if (is.ggplot(data$plot_m_3)) {
        if (output_folder != "none") { png(paste(output_folder, "heatmap_m_3.png",  sep="/"), width=800, height=800*data$ratio, res=100) }
        print(data$plot_m_3)
        if (output_folder != "none") { o=dev.off() }
    }

    if (is.ggplot(data$plot_m_5)) {
        if (output_folder != "none") { png(paste(output_folder, "heatmap_m_5.png",  sep="/"), width=800, height=800*data$ratio, res=100) }
        print(data$plot_m_5)
        if (output_folder != "none") { o=dev.off() }
    }

    if (is.ggplot(data$plot_m_10)) {
        if (output_folder != "none") { png(paste(output_folder, "heatmap_m_10.png",  sep="/"), width=800, height=800*data$ratio, res=100) }
        print(data$plot_m_10)
        if (output_folder != "none") { o=dev.off() }
    }
}



aggregate_heatmaps_plot = function(data, output_file, title = "none", species="none", show.significance = TRUE) {

    if (title == "none" & species == "none") {title="Haplotypes distribution"}
    title = get_title(species=species, base=title)

    if (data$n_plots < 4) {
        width_c = 800 * data$n_plots
        if (output_file != "none") { png(output_file, width=width_c, height=width_c*(1/data$n_plots)*data$ratio, res=140) }
    } else {
        width_c = 1600
        if (output_file != "none") { png(output_file, width=width_c, height=width_c*data$ratio, res=140) }
    }

    if (is.ggplot(data$plot_m_5)) {
        if (is.ggplot(data$plot_m_10)) {
            if (is.ggplot(data$plot_m_3)) {
                if(is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=2, nrow=2, top=textGrob(title, gp=gpar(fontsize=15)), right=g_legend(data$plot_m_2),
                                 widths = c(data$n_males, data$n_males), heights = c(data$n_females, data$n_females))
                } else {
                    grid.arrange(data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=3, nrow=1, top=textGrob(title, gp=gpar(fontsize=15)), right=g_legend(data$plot_m_3),
                                 widths = c(data$n_males, data$n_males, data$n_males), heights = c(data$n_females))
                }
            } else {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=3, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_2),
                                 widths = c(data$n_males, data$n_males, data$n_males), heights = c(data$n_females))
                } else {
                    grid.arrange(data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=2, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_5),
                                 widths = c(data$n_males, data$n_males), heights = c(data$n_females))
                }
            }
        } else {
            if (is.ggplot(data$plot_m_3)) {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 ncol=3, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_2),
                                 widths = c(data$n_males, data$n_males, data$n_males), heights = c(data$n_females))
                } else {
                    grid.arrange(data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 ncol=2, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_3),
                                 widths = c(data$n_males, data$n_males), heights = c(data$n_females))
                }

            } else {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 ncol=2, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_2),
                                 widths = c(data$n_males, data$n_males), heights = c(data$n_females))
                } else {
                    print(data$plot_m_5)
                }
            }
        }
    } else {
        if (is.ggplot(data$plot_m_10)) {
            if (is.ggplot(data$plot_m_3)) {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=3, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_2),
                                 widths = c(data$n_males, data$n_males, data$n_males), heights = c(data$n_females))
                } else {
                    grid.arrange(data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=2, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_3),
                                 widths = c(data$n_males, data$n_males), heights = c(data$n_females))
                }
            } else {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=2, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_2),
                                 widths = c(data$n_males, data$n_males), heights = c(data$n_females))
                } else {
                    print(data$plot_m_10)
                }
            }
        } else {
            if (is.ggplot(data$plot_m_3)) {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_3 + theme(legend.position="none"),
                                 ncol=2, nrow=1, top=textGrob(title, gp=gpar(fontsize=20)), right=g_legend(data$plot_m_2),
                                 widths = c(data$n_males, data$n_males), heights = c(data$n_females))
                } else {
                    print(data$plot_m_3)
                }
            } else {
                print(data$plot_m_2)
            }
        }
    }
    if (output_file != "none") { o=dev.off() }
}



generate_heatmap = function(haplotypes_matrix, show.significance=TRUE, title="none") {

    bins = c(0, 1, 5, 25, 100, 1000)
    bin_labels = c('0', '1-4', '5-24', '25-99', '100-999', '>1000')
    get_palette = colorRampPalette(c("white", "navyblue"))
    palette = get_palette(length(bin_labels))
    names(palette) = bins

    n_males = max(haplotypes_matrix$Males)
    n_females = max(haplotypes_matrix$Females)
    threshold = - log(0.05 / sum(haplotypes_matrix$Loci), 10)

    group = c()
    for (i in 1:dim(haplotypes_matrix)[1]) {
        group = c(group, bins[tail(which(haplotypes_matrix$Loci[i]>=bins), n=1)])
    }
    haplotypes_matrix$Bin = factor(group, levels=bins)

    g = ggplot(haplotypes_matrix, aes(x=Males, y=Females)) +
        geom_tile(aes(fill=Bin), color="grey70") +
        theme(plot.title = element_text(hjust = 0.5)) +
        scale_fill_manual(name="Number of\nhaplotypes", breaks=bins, values = palette, labels = bin_labels, drop=FALSE) +
        scale_x_continuous(breaks = seq(0, n_males, 5), minor_breaks = seq(0, n_males, 1)) +
        scale_y_continuous(breaks = seq(0, n_females, 5), minor_breaks = seq(0, n_females, 1))

    if (show.significance) {
        signif = c()
        temp = data.frame(Present=c(0, 0), Absent=c(0, 0))
        for (i in 1:dim(haplotypes_matrix)[1]) {
            temp$Present = c(haplotypes_matrix$Males[i], haplotypes_matrix$Females[i])
            temp$Absent = c(n_males-haplotypes_matrix$Males[i], n_females-haplotypes_matrix$Females[i])
            x = suppressWarnings(-log(chisq.test(temp)$p.value, 10))
            signif = c(signif, x>threshold)
        }
        haplotypes_matrix$Signif = signif
        g = g + geom_tile(data=haplotypes_matrix, aes(x=Males, y=Females, color=Signif), fill="NA", size=0.3) +
            scale_color_manual(name=element_blank(), values=c("TRUE"="black", "FALSE"="NA"), breaks = c("TRUE"), labels = c("Association\nwith sex"))
    }

    if (title != "none") {
        g = g + ggtitle(title)
    }

    return(g)
}
