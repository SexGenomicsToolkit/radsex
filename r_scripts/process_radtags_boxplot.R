source("/home/rferon/work/analyses/radseq_all/r_scripts/utils.R")
library(cowplot)


to_M = function(x) {
    return(x/1000000)
}


process_radtags_plot = function(data_file, popmap_file, output_file="none", species="none") {

    if (species != "none") {
        title = paste("Retained reads for each sex in \n", format_species(species), sep="")
    } else {
        title = "Process radtags results"
    }

    data = suppressMessages(read_delim(data_file, "\t", escape_double = FALSE, trim_ws = TRUE, col_names=FALSE))
    names(data) = c("Barcode", "Individual", "Total_reads", "Retained_reads")

    popmap = load_popmap_file(popmap_file)
    data$Sex = rep('M', dim(data)[1])
    for (i in 1:dim(data)[1]) {
        data$Sex[i] = popmap$Sex[which(popmap$Individual == data$Individual[i])]
    }
    data$Sex = factor(data$Sex, levels=c("M", "F", "N"))

    g = ggplot(data, aes(x=Sex, y=Retained_reads, fill=Sex)) +
        stat_boxplot(geom="errorbar", alpha=1, width=0.35) +
        geom_boxplot(alpha=1, fill="white", outlier.alpha=0) +
        geom_boxplot(alpha=0.7, outlier.alpha=0, color="black") +
        geom_jitter(width=0.1, size=1.5, alpha=0.9) +
        theme(plot.title = element_text(hjust = 0.5), legend.position="none",
              axis.title.x = element_blank(), axis.title.y = element_text(margin=margin(0,10,0,0)),
              panel.grid.major.x = element_blank()) +
        scale_fill_manual(name="Sex", values = c("M"="dodgerblue3", "F"="firebrick2", "N"="grey55")) +
        scale_x_discrete(labels=c("Males", "Females", "Undet.")) +
        scale_y_continuous(name="Million retained reads", labels=to_M) +
        ggtitle(title)

    return(g)
}

species_list = c("acipenser_ruthenus", "danio_aesculapii", "danio_albolineatus", "danio_choprae", "danio_kyathit",
                 "gadus_morhua", "gymnocorymbus_ternetzi", "gymnotus_carapo", "hippocampus_abdominalis", "lepisosteus_oculatus",
                 "notothenia_rossi", "plecoglossus_altivelis", "poecilia_sphenops", "sander_vitreus", "cyprinus_carpio", "tinca_tinca")

for (i in 1:length(species_list)) {
    species=species_list[i]
    data_file = paste("/home/rferon/work/writing/radsex/data/phylosex_results/", species, "/process_radtags.tsv", sep="")
    popmap_file = paste("/home/rferon/work/writing/radsex/data/phylosex_results/", species, "/popmap.tsv", sep="")
    output_file = paste("/home/rferon/work/writing/radsex/data/phylosex_results/", species, "/coverage_boxplot", sep="")
    g = process_radtags_plot(data_file, popmap_file, output_file=output_file, species=species)
    save_plot(filename = paste(output_file, ".png", sep=""), plot=g)
    save_plot(filename = paste(output_file, ".svg", sep=""), plot=g)
}




