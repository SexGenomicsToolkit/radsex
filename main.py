import os
from radseq_analyses_pipeline import analyse_directory

root_dir = '/home/rferon/work/code/radseq_analyses_pipeline/'
files_dir = os.path.join(root_dir, 'results_m_5_n_1_M_3')
output_dir = os.path.join(root_dir, 'output')

analyse_directory(files_dir, output_dir)
