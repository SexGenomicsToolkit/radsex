import os
from radseq_analyses_pipeline import analyse_directory, visualise_directory

root_dir = '/home/rferon/work/code/radseq_analyses_pipeline/data'
files_dir = os.path.join(root_dir, 'results_m_5_n_1_M_3')
output_dir = os.path.join(root_dir, 'output')

# Maximum proportion of individuals deviating from the population
error_threshold = 0.1

# analyse_directory(files_dir, output_dir, error_threshold, visualize=True)
visualise_directory(files_dir, output_dir, error_threshold)
