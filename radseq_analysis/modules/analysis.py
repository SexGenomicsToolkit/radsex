import os
from radseq_analysis.shared import Parameters
from radseq_analysis.modules import sex_linked_haplotypes
from radseq_analysis.modules import loci_matrix
from radseq_analysis.file_handler import load_popmap
from radseq_analysis.file_handler import load_positions_list


def analysis(input_dir=None,
             popmap_file_path=None,
             output_file_path=None,
             positions_file_path=None,
             analysis=None):

    parameters = Parameters(files_dir=input_dir,
                            output_file_path=output_file_path)

    load_popmap(popmap_file_path, parameters)

    if positions_file_path:
        load_positions_list(positions_file_path, parameters)

    # TODO: support more batch numbers
    haplotypes_file_path = os.path.join(input_dir, 'batch_0.haplotypes.tsv')
    catalog_file_path = os.path.join(input_dir, 'batch_0.catalog.tags.tsv.gz')

    if analysis == 'heatmap':
        loci_matrix(haplotypes_file_path, parameters)
    elif analysis == 'haplotypes':
        sex_linked_haplotypes(haplotypes_file_path, catalog_file_path, parameters)
