#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

# test if there is at least one argument: if not, return an error
if (length(args)==0) {
    stop("At least one argument must be supplied (input file).", call.=FALSE)
} else if (length(args) != 2){
    stop("Usage: R frequencies.R input_file.tsv output_dir")
}

suppressMessages(library(readr))
suppressMessages(library(ggplot2))
suppressMessages(library(svglite))
suppressMessages(library(scales))

file = args[1]
output_dir = args[2]

png_name = paste("tags_distribution.png", sep='')

data = suppressMessages(read_delim(file, "\t", escape_double = FALSE, col_names = TRUE, trim_ws = TRUE))
colnames(data) = c("Frequency", "Count")

g = ggplot(data, aes(x=Frequency, y=Count/sum(Count))) +
    geom_bar(stat="identity", colour="black", fill="#CCCCCC") +
    geom_vline(xintercept = mean(data$Frequency), colour = "red") +
    geom_vline(xintercept = median(data$Frequency), colour = "blue") +
    ggtitle(paste("Total tags: ", dim(data)[1], sep='')) + theme(plot.title = element_text(hjust = 0.5)) +
    xlab("Number of individuals in which a tag is present") + ylab("Frequency (%)") +
    scale_y_continuous(labels=percent)

png(paste(output_dir, png_name, sep=''), width=1600, height=1000, res=130)
print(g)
x = dev.off()
