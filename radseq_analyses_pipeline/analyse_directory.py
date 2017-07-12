import os
from .shared import Parameters
from .modules import extraction
from .file_handler import load_popmap


def analyse_directory(files_dir=None, analyses_dir=None, positions_list=None,
                      popmap_path=None, visualize=False):

    if not os.path.isdir(analyses_dir):
        os.mkdir(analyses_dir)

    parameters = Parameters(files_dir=files_dir,
                            results_dir=analyses_dir,
                            positions_list=positions_list)

    load_popmap(popmap_path, parameters)

    haplotypes_file_path = os.path.join(files_dir, 'batch_0.haplotypes.tsv')
    catalog_file_path = os.path.join(files_dir, 'batch_0.catalog.tags.tsv.gz')

    extraction(haplotypes_file_path, catalog_file_path, parameters)
