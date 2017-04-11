import os
from . import r_plots


def visualise(output_dir, results_dir, threshold):

    results_dir = os.path.join(results_dir, '')
    frequencies_file = os.path.join(output_dir, 'frequencies_data.tsv')
    haplotypes_file = os.path.join(output_dir, 'haplotypes_data.tsv')
    print(' - Plotting frequencies data')
    r_plots.frequencies(frequencies_file, results_dir)
    print(' - Plotting haplotypes data')
    r_plots.haplotypes(haplotypes_file, results_dir, threshold)
