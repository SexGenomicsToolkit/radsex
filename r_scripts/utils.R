library(readr)
library(ggplot2)
library(reshape2)
library(ggdendro)
library(grid)
library(gridExtra)
library(gtable)
library(scales)


g_legend = function(a.gplot) {

    tmp = ggplot_gtable(ggplot_build(a.gplot))
    leg = which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
    legend = tmp$grobs[[leg]]
    return(legend)
}


load_popmap_file = function(popmap_file) {

    popmap = suppressMessages(read_delim(popmap_file, "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE))
    names(popmap) = c('Individual', 'Sex')

    return(popmap)
}


load_coverage_file = function(coverage_file) {

    data = suppressMessages(read_delim(coverage_file, "\t", escape_double = FALSE, trim_ws = TRUE))
    data$Stack_ID = factor(data$Stack_ID)
    data$Haplotype_ID = factor(data$Haplotype_ID)

    return(data)
}


load_haplotypes_matrix = function(haplotypes_matrix_path) {

    data = suppressMessages(read_delim(haplotypes_matrix_path, "\t", escape_double = FALSE, col_names = FALSE, trim_ws = TRUE))
    males = rep(seq(0, dim(data)[2] - 1), dim(data)[1])
    females = c()
    loci = c()
    for (i in 1:dim(data)[1]){
        females = c(females, rep(i-1, dim(data)[2]))
        loci = c(loci, as.vector(unlist(data[i,])))
    }
    d = data.frame('Males' = males, 'Females' = females, 'Loci' = loci)

    return(d)
}


format_species = function(species, short=FALSE) {

    formatted = ""

    temp = strsplit(species, "_")[[1]]
    temp[1] = paste(toupper(substring(temp[1], 1,1)), substring(temp[1], 2), sep="", collapse=" ")
    if (short) {
        temp[1] = paste(substr(temp[1], 1, 1), ".", sep="")
    }
    formatted = paste(temp[1], temp[2])
    if (length(temp) == 3) {
        formatted = paste(formatted, temp[3])
    }

    return(formatted)
}



get_title = function(species="none", m_value="none", base="none") {

    if (base == "") {base = "none"}
    plot_title = base

    if (species != "none") {
        if (plot_title == "none") {
            plot_title = format_species(species)
            if (m_value != "none") {
                plot_title = paste(plot_title, " (m=", m_value, ")", sep="")
            }
        } else {
            plot_title = paste(plot_title, " (", format_species(species), sep="")
            if (m_value != "none") {
                plot_title = paste(plot_title, ", m=", m_value, ")", sep="")
            } else {
                plot_title = paste(plot_title, ")", sep="")
            }
        }
    } else {
        if (plot_title == "none") {
            if (m_value != "none") {
                plot_title = paste("m=", m_value, sep="")
            } else {
                plot_title = ""
            }
        } else {
            if (m_value != "none") {
                plot_title = paste(plot_title, " (m=", m_value, ")", sep="")
            }
        }
    }

    return(plot_title)
}

