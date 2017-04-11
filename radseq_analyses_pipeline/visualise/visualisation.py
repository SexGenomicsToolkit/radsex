import os
from . import r_plots


def visualise(global_parameters):

    results_dir = os.path.join(global_parameters.results_dir, '')
    print(' - Plotting frequencies data')
    r_plots.frequencies(global_parameters.frequencies_file, results_dir)
    print(' - Plotting haplotypes data')
    r_plots.haplotypes(global_parameters.haplotypes_file, results_dir, global_parameters.error_threshold)
