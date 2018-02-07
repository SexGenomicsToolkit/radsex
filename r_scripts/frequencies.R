source("/home/rferon/work/analyses/radseq_all/r_scripts/utils.R")


aggregate_frequencies = function(input_folder, output_file="none", title = "none", species="none") {

    input_folder = paste(sub("/$", "", input_folder))
    data = parse_folder(input_folder)
    aggregate_frequencies_plot(data, output_file=output_file, title=title, species=species)
}



parse_folder = function(folder) {

    output = list()
    plot_m_2 = single_plot(folder, 2)
    plot_m_3 = single_plot(folder, 3)
    plot_m_5 = single_plot(folder, 5)
    plot_m_10 = single_plot(folder, 10)

    n_plots = 0
    if (is.ggplot(plot_m_2)) {n_plots = n_plots + 1}
    if (is.ggplot(plot_m_3)) {n_plots = n_plots + 1}
    if (is.ggplot(plot_m_5)) {n_plots = n_plots + 1}
    if (is.ggplot(plot_m_10)) {n_plots = n_plots + 1}

    output$plot_m_2 = plot_m_2
    output$plot_m_3 = plot_m_3
    output$plot_m_5 = plot_m_5
    output$plot_m_10 = plot_m_10
    output$n_plots = n_plots

    return(output)
}



single_plot = function(folder, m_value) {

    frequencies_file_path = paste(folder, paste("m_", m_value, "_frequencies.tsv", sep=""), sep="/")

    if (file.exists(frequencies_file_path)){
        g = generate_plot(frequencies_file_path, title = get_title(m_value=m_value))
    } else {
        print(paste("Frequencies file", frequencies_file_path, "does not exist.", sep=" "))
        g = "none"
    }

    return(g)
}



aggregate_frequencies_plot = function(data, output_file="none", title="none", species="none") {

    width_c = 1600
    height_c = 500

    if (title == "none" & species == "none") {title="Haplotypes frequencies"}
    title = get_title(species=species, base=title)

    if (output_file != "none") { png(output_file, width=width_c, height=height_c*data$n_plots, res=160) }

    if (is.ggplot(data$plot_m_5)) {
        if (is.ggplot(data$plot_m_10)) {
            if (is.ggplot(data$plot_m_3)) {
                if(is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=1, nrow=4, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                } else {
                    grid.arrange(data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=1, nrow=3, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                }
            } else {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=1, nrow=3, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                } else {
                    grid.arrange(data$plot_m_5 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=1, nrow=2, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                }
            }
        } else {
            if (is.ggplot(data$plot_m_3)) {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 ncol=1, nrow=3, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                } else {
                    grid.arrange(data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 ncol=1, nrow=2, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                }

            } else {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_5 + theme(legend.position="none"),
                                 ncol=1, nrow=2, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
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
                                 ncol=1, nrow=3, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                } else {
                    grid.arrange(data$plot_m_3 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=1, nrow=2, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                }
            } else {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_10 + theme(legend.position="none"),
                                 ncol=1, nrow=2, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
                } else {
                    print(data$plot_m_10)
                }
            }
        } else {
            if (is.ggplot(data$plot_m_3)) {
                if (is.ggplot(data$plot_m_2)) {
                    grid.arrange(data$plot_m_2 + theme(legend.position="none"),
                                 data$plot_m_3 + theme(legend.position="none"),
                                 ncol=1, nrow=2, top=textGrob(title, gp=gpar(fontsize=15)),
                                 bottom="Number of individuals")
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



generate_plot = function(file_path, title="none") {

    data = suppressMessages(read_delim(file_path, "\t", escape_double = FALSE, col_names = TRUE, trim_ws = TRUE))

    data$Freq_p = data$Frequency * 100 / max(data$Frequency)
    data$Count_p = data$Count * 100 / sum(data$Count)

    g = ggplot(data, aes(x=Freq_p, y=Count_p)) +
        geom_bar(stat="identity", color="black", fill="grey") +
        scale_y_continuous(name="% of stacks") +
        theme(plot.title = element_text(hjust=0.5), axis.title.x = element_blank()) +
        annotate("text", x=50, y=3*max(data$Count_p/4), label=paste("Total number of stacks: ", format(sum(data$Count), big.mark = ",")))

    if (title != "none") {
        g = g + ggtitle(title)
    }

    return(g)
}
