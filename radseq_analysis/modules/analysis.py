from radseq_analysis.shared import Parameters
from radseq_analysis.modules import sex_linked_haplotypes
from radseq_analysis.modules import loci_matrix
from radseq_analysis.modules import stacks_privacy
from radseq_analysis.modules import rescue
from radseq_analysis.modules import individual_coverage
from radseq_analysis.modules import visualization


def analysis(input_dir=None,
             popmap_file_path=None,
             output_file_path=None,
             positions_file_path=None,
             sequences_file_path=None,
             coverage_file_path=None,
             markers_file_path=None,
             analysis=None):

    parameters = Parameters(files_dir=input_dir,
                            popmap_file_path=popmap_file_path,
                            output_file_path=output_file_path,
                            positions_file_path=positions_file_path,
                            sequences_file_path=sequences_file_path,
                            coverage_file_path=coverage_file_path,
                            markers_file_path=markers_file_path)

    parameters.get_files(input_dir)

    if analysis == 'heatmap':
        loci_matrix(parameters)
    elif analysis == 'haplotypes':
        sex_linked_haplotypes(parameters)
    elif analysis == 'frequencies':
        stacks_privacy(parameters)
    elif analysis == 'rescue':
        rescue(parameters)
    elif analysis == 'coverage':
        individual_coverage(parameters)
    elif analysis == 'visualize':
        visualization(parameters)
