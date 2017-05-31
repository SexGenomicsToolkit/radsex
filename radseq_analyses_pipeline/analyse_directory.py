import os
from .parameters import Parameters
from . import analyses, visualisation


def analyse_directory(root_dir, files_dir, analyses_dir, error_threshold, n_threads, visualize):

    if not os.path.isdir(analyses_dir):
        os.mkdir(analyses_dir)
    output_dir = os.path.join(analyses_dir, 'output')
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)
    results_dir = os.path.join(analyses_dir, 'results')
    if not os.path.isdir(results_dir):
        os.mkdir(results_dir)

    parameters = Parameters(root_dir=root_dir,
                            files_dir=files_dir,
                            output_dir=output_dir,
                            results_dir=results_dir,
                            error_threshold=error_threshold,
                            n_threads=n_threads)

    analyses.run(parameters)

    if visualize:
        visualisation.run(parameters)


def visualise_directory(files_dir, analyses_dir, error_threshold):

    output_dir = os.path.join(analyses_dir, 'output')
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)
    results_dir = os.path.join(analyses_dir, 'results')
    if not os.path.isdir(results_dir):
        os.mkdir(results_dir)

    parameters = Parameters(files_dir=files_dir,
                            output_dir=output_dir,
                            results_dir=results_dir,
                            error_threshold=error_threshold)

    visualisation.run(parameters)
