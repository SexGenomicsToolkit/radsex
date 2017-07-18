from radseq_analysis import file_handler
from radseq_analysis import output


def analysis(catalog_file_path, global_parameters):

    frequencies = file_handler.get_info_from_catalog(catalog_file_path,
                                                     consensus=False,
                                                     frequencies=True)

    output.stacks_privacy(global_parameters.output_file_path, frequencies)
