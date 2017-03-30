#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

# test if there is at least one argument: if not, return an error
if (length(args)==0) {
    stop("At least one argument must be supplied (input file).n", call.=FALSE)
} else if (length(args) != 3){
    stop("Usage: R ggplot.R input_file.tsv output_dir n_individuals")
}

suppressMessages(library(readr))
suppressMessages(library(ggplot2))
suppressMessages(library(svglite))
suppressMessages(library(scales))

file = args[1]
name = strsplit(file, '/')[[1]][length(strsplit(file, '/')[[1]])]
output_dir = args[2]
n_max = strtoi(args[3])

split = strsplit(name, '_')[[1]]
species_name = paste(split[1], "_", split[2], sep="")
png_name = paste(species_name, "_", split[4], ".png", sep='')
data = suppressMessages(read_delim(file, "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE))
species = rep(species_name, dim(data)[1])
m_value = rep(split[4],dim(data)[1])
colnames(data) = c("Stack_ID", "Frequency")

data = subset(data, data$Frequency <= n_max)

g = ggplot(data, aes(x=Frequency)) +
    geom_bar(aes(y = (..count..)/sum(..count..)), colour="black", fill="#CCCCCC") +
    geom_vline(xintercept = mean(data$Frequency), colour = "red") +
    geom_vline(xintercept = median(data$Frequency), colour = "blue") +
    ggtitle(paste("Total tags: ", dim(data)[1], sep='')) + theme(plot.title = element_text(hjust = 0.5)) +
    xlab("Number of individuals in which a tag is present") + ylab("Frequency (%)") +
    scale_y_continuous(labels=percent)

png(paste(output_dir, gsub(".tsv", "", name), ".png", sep=''), width=1600, height=800, res=100)
print(g)
x = dev.off()
