import os
from .parameters import Parameters
from . import analyses
from .popmap import get_popmap


def analyse_directory(root_dir=None, files_dir=None, analyses_dir=None, positions_list=0,
                      popmap_path=None, visualize=False):

    if not os.path.isdir(analyses_dir):
        os.mkdir(analyses_dir)
    output_dir = os.path.join(analyses_dir, 'results')
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)
    results_dir = os.path.join(analyses_dir, 'figures')
    if not os.path.isdir(results_dir):
        os.mkdir(results_dir)

    popmap = get_popmap(popmap_path)

    parameters = Parameters(root_dir=root_dir,
                            files_dir=files_dir,
                            output_dir=output_dir,
                            results_dir=results_dir,
                            positions_list=positions_list,
                            popmap=popmap)

    analyses.run(parameters)
