from radseq_analysis import file_handler
from radseq_analysis import output


def analysis(parameters):

    frequencies = file_handler.get_info_from_catalog(parameters.catalog_file_path,
                                                     consensus=False,
                                                     frequencies=True)

    output.stacks_privacy(parameters.output_file_path, frequencies)
