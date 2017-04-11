#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

# test if there is at least one argument: if not, return an error
if (length(args)==0) {
    stop("At least one argument must be supplied (input file).n", call.=FALSE)
} else if (length(args) != 3){
    stop("Usage: R haplotypes.R input_file.tsv output_dir treshold")
}

suppressMessages(library(readr))
suppressMessages(library(ggplot2))
suppressMessages(library(gridExtra))

file = args[1]
output_dir = args[2]
threshold = args[3]

png_name = paste("sex_variable_haplotypes.png", sep='')

data <- suppressMessages(read_delim(file, "\t", col_names = TRUE, escape_double = FALSE, trim_ws = TRUE))
names(data) = c('Locus', 'Sequence', 'Males', 'Females', 'Male_outliers', 'Female_outliers')

males = data.frame(table(data$Males[which(data$Males > threshold)], data$Females[which(data$Males > threshold)]))
names(males) = c('Males', 'Females', 'Count')

females = data.frame(table(data$Males[which(data$Females > threshold)], data$Females[which(data$Females > threshold)]))
names(females) = c('Males', 'Females', 'Count')

g <- ggplot(males, aes(x = Males, y = Count, fill = Females))
g = g + geom_bar(stat='identity', position=position_dodge(), colour='black')

h <- ggplot(females, aes(x = Females, y = Count, fill = Males))
h = h + geom_bar(stat='identity', position=position_dodge(), colour='black')

png(paste(output_dir, png_name, sep=''), width=1600, height=1000, res=130)
grid.arrange(g, h, ncol=2)
x = dev.off()

