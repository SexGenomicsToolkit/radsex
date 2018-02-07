source("/home/rferon/work/analyses/radseq_all/r_scripts/heatmap.R")

species_name = "Australia"
folder = "/home/rferon/work/analyses/radseq_all/extra/seahorse/australia/"
heatmaps_output = paste(folder, "heatmaps.png", sep="")

aggregate_heatmaps(folder, show.significance = TRUE, output_file = heatmaps_output,
                   species = species_name, title="none")
separate_heatmaps(folder, show.significance = TRUE, title="none", output_folder = folder, species = species_name)
