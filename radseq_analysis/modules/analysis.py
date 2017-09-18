import os
from radseq_analysis.shared import Parameters
from radseq_analysis.modules import sex_linked_haplotypes
from radseq_analysis.modules import loci_matrix
from radseq_analysis.modules import stacks_privacy
from radseq_analysis.modules import rescue
from radseq_analysis.modules import association
from radseq_analysis.modules import visualization
from radseq_analysis.file_handler import load_popmap
from radseq_analysis.file_handler import load_positions_list


def analysis(input_dir=None,
             input_file_path=None,
             popmap_file_path=None,
             output_file_path=None,
             positions_file_path=None,
             sequences_file_path=None,
             coverage_file_path=None,
             analysis=None):

    parameters = Parameters(files_dir=input_dir,
                            output_file_path=output_file_path)

    species = None
    if popmap_file_path:
        load_popmap(popmap_file_path, parameters)
        species = os.path.split(popmap_file_path)[1]
        species = os.path.splitext(species)[0]
        if len(species.split('_')) > 1:
            species = '_'.join(s for s in species.split('_')[:-1])

    parameters.species = species

    if positions_file_path:
        load_positions_list(positions_file_path, parameters)

    # TODO: support more batch numbers + move this inside analyses?
    if input_dir:
        haplotypes_file_path = os.path.join(input_dir, 'batch_0.haplotypes.tsv')
        catalog_file_path = os.path.join(input_dir, 'batch_0.catalog.tags.tsv.gz')
        individual_files_paths = [os.path.join(input_dir, f) for
                                  f in os.listdir(input_dir) if
                                  'tags' in f and 'catalog' not in f]

    if analysis == 'heatmap':
        loci_matrix(haplotypes_file_path, parameters)
    elif analysis == 'haplotypes':
        sex_linked_haplotypes(haplotypes_file_path, catalog_file_path, parameters)
    elif analysis == 'frequencies':
        stacks_privacy(catalog_file_path, parameters)
    elif analysis == 'rescue':
        rescue(sequences_file_path, catalog_file_path, individual_files_paths, coverage_file_path, parameters)
    elif analysis == 'visualize':
        visualization(input_file_path, popmap_file_path, output_file_path, parameters)
    elif analysis == 'association':
        association(haplotypes_file_path, parameters)
