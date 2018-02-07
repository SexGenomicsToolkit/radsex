source("/home/rferon/work/analyses/radseq_all/r_scripts/heatmap.R")

species = "amia_calva"

input_file = "/home/rferon/work/code/radsex/matrix.tsv"
output_file = "/home/rferon/work/code/radsex/heatmap"

heatmap(input_file, output_file, show.significance=TRUE, width_c=1200)
