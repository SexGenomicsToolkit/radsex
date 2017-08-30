#!/usr/bin/env Rscript
suppressMessages(require(readr))
suppressMessages(require(ggplot2))
suppressMessages(require(base))

args = commandArgs(trailingOnly=FALSE)
nargs = length(args)

# R script arguments are last in arguments list
input_file_path = args[nargs - 2]
popmap_file_path = args[nargs - 1]
output_dir_path = args[nargs]

if (substr(output_dir_path, nchar(output_dir_path), nchar(output_dir_path)) != "/") {
    output_dir_path = cat(output_dir_path, "/")
}

script_dir = ""

for (i in 1:nargs) {
    if (substr(args[i], 1, 6) == "--file") {
        full_path = substr(args[i], 8, nchar(args[i]))
        split_path = strsplit(full_path, "/")
        script_dir = paste(split_path[[1]][-length(split_path[[1]])], collapse="/")
    }
}

source(paste(script_dir, "clustering.R", sep="/"), chdir=T)

png(paste(output_dir_path, "coverage_heatmap.png", sep=""), width=2400, height=1200, res=90)
coverage_heatmap(input_file_path, popmap_file_path, distance="euclidian", clustering="ward.D")
x = dev.off()

png(paste(output_dir_path, "presence_heatmap.png", sep=""), width=2400, height=1200, res=90)
presence_heatmap(input_file_path, popmap_file_path, distance="binary", clustering="ward.D")
x = dev.off()

