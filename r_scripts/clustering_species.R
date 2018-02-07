source("/home/rferon/work/analyses/radseq_all/r_scripts/clustering.R")

species = "esox_masquinongy_1"

coverage_file = paste("~/work/analyses/radseq_all/results/rescue/", species, ".tsv", sep="")
popmap_file = paste("~/work/analyses/radseq_all/data/popmaps/", species, "_popmap.csv", sep="")
presence_output_file = paste("~/work/analyses/radseq_all/figures/clustering/", species, "_presence.png", sep="")
coverage_output_file = paste("~/work/analyses/radseq_all/figures/clustering/", species, "_coverage.png", sep="")

# png(coverage_output_file, width=2400, height=1200, res=100)
coverage_heatmap(coverage_file, popmap_file, distance="euclidian", clustering="ward.D")
# dev.off()

# png(presence_output_file, width=2400, height=1200, res=100)
# presence_heatmap(coverage_file, popmap_file, distance="binary", clustering="ward.D", read.length=96)
# dev.off()

