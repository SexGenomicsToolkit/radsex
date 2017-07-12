import os
from .shared import Parameters
from . import analyses
from .popmap import get_popmap


def analyse_directory(files_dir=None, analyses_dir=None, positions_list=0,
                      popmap_path=None, visualize=False):

    if not os.path.isdir(analyses_dir):
        os.mkdir(analyses_dir)

    popmap = get_popmap(popmap_path)

    parameters = Parameters(files_dir=files_dir,
                            output_dir=analyses_dir,
                            positions_list=positions_list,
                            popmap=popmap)

    analyses.run(parameters)
