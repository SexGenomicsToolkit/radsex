import os
from radseq_analyses_pipeline import analyse_directory

root_dir = '/home/rferon/work/code/radseq_analyses_pipeline/'
files_dir = os.path.join(root_dir, 'data', 'astyanax_mexicanus_1')
analyses_dir = os.path.join(root_dir, 'data', 'analyses')

# Maximum proportion of individuals deviating from the population
error_threshold = 0.1

# Number of threads to use when possible
n_threads = 4

# Path to population map
popmap_path = os.path.join(root_dir, 'data', 'astyanax_mexicanus_1_popmap.csv')

analyse_directory(root_dir, files_dir, analyses_dir,
                  error_threshold, n_threads, popmap_path)
