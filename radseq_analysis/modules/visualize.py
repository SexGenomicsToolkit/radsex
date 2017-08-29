from radseq_analysis import visualization


def detect_plot_type(input_file):
    return 'haplotypes'


def analysis(input_file, output_file, parameters):

    plot_type = detect_plot_type(input_file)

    if plot_type == 'haplotypes':
        visualization.haplotypes(input_file, output_file, parameters.species)
