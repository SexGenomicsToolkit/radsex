source("/home/rferon/work/analyses/radseq_all/r_scripts/utils.R")


process_radtags_plot = function(data_file, popmap_file, output_file="none", species="none") {

    if (species != "none") {
        title = format_species(species)
    } else {
        title = "Process radtags results"
    }

    removal_palette = c("TRUE"="black", "FALSE"="firebrick3")

    data = suppressMessages(read_delim(data_file, "\t", escape_double = FALSE, trim_ws = TRUE, col_names=FALSE))
    names(data) = c("Barcode", "Individual", "Total_reads", "Retained_reads")

    popmap = load_popmap_file(popmap_file)
    data$Sex = rep('M', dim(data)[1])
    for (i in 1:dim(data)[1]) {
        data$Sex[i] = popmap$Sex[which(popmap$Individual == data$Individual[i])]
    }
    data$Sex = factor(data$Sex, levels=c("M", "F", "N"))

    data = data[order(data$Retained_reads),]
    data$Individual = factor(data$Individual, levels=data$Individual)
    data$Removal = factor(data$Retained_reads < 0.1 * mean(data$Retained_reads))

    g = ggplot(data, aes(x=Individual, y=Retained_reads, fill=Sex)) +
        geom_bar(stat="identity", width=0.75, color="grey60") +
        theme(plot.title = element_text(hjust = 0.5), axis.text.x  = element_text(angle=90, vjust=0.5, size=10, color=removal_palette[data$Removal]),
              axis.title.x = element_blank(), panel.grid.major.x = element_blank()) +
        scale_fill_manual(name="Sex", values = c("M"="steelblue2", "F"="lightgoldenrod1", "N"="grey55"), labels = c("Male", "Female", "Unkown")) +
        scale_y_continuous(name="Retained reads") +
        geom_hline(yintercept = mean(data$Retained_reads), color="black") +
        annotate("text", x=5, y=1.025*mean(data$Retained_reads), label="Average") +
        geom_hline(yintercept = 0.1*mean(data$Retained_reads), color="black", linetype=2) +
        annotate("text", x=5, y=0.125*mean(data$Retained_reads), label="Removal threshold") +
        ggtitle(title)

    if (output_file != "none") { png(output_file, width=1600, height=1000, res=100) }
    print(g)
    if (output_file != "none") { o=dev.off() }
}



