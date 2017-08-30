from radseq_analysis import visualization


def detect_plot_type(input_file):
    return 'clustering'


def analysis(input_file_path, popmap_file_path, output_file_path, parameters):

    plot_type = detect_plot_type(input_file_path)

    if plot_type == 'haplotypes':
        visualization.haplotypes(input_file_path, output_file_path, parameters.species)
    elif plot_type == 'clustering':
        visualization.clustering(input_file_path, popmap_file_path, output_file_path)
