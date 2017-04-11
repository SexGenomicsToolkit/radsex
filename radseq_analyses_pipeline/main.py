import os
from analyse.analysis import analyse_directory

root_dir = '/home/rferon/work/code/radseq_analyses_pipeline/'
files_dir = os.path.join(root_dir, 'results_10')
output_dir = os.path.join(root_dir, 'output')

analyse_directory(files_dir, output_dir)
