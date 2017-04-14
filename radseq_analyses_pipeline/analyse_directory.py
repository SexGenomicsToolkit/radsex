import os
from .parameters import Parameters
from . import analyses


def analyse_directory(files_dir, analyses_dir):

    output_dir = os.path.join(analyses_dir, 'output')
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)
    results_dir = os.path.join(analyses_dir, 'results')
    if not os.path.isdir(results_dir):
        os.mkdir(results_dir)

    error_threshold = 0.1

    parameters = Parameters(files_dir=files_dir,
                            output_dir=output_dir,
                            results_dir=results_dir,
                            error_threshold=error_threshold)

    analyses.run(parameters)
