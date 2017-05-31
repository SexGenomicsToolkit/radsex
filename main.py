import os
from radseq_analyses_pipeline import analyse_directory
# from radseq_analyses_pipeline import visualise_directory

root_dir = '/home/rferon/work/code/radseq_analyses_pipeline/'
files_dir = os.path.join(root_dir, 'data', 'results_m_5_n_1_M_3')
analyses_dir = os.path.join(root_dir, 'data', 'analyses')

# Maximum proportion of individuals deviating from the population
error_threshold = 0.1

# Number of threads to use when possible
n_threads = 4

popmap_path = os.path.join(root_dir, 'data', 'popmap.tsv')

analyse_directory(root_dir, files_dir, analyses_dir,
                  error_threshold, n_threads, popmap_path)
# visualise_directory(files_dir, analyses_dir, error_threshold)
