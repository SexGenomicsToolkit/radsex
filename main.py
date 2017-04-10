import os
from analyse.analysis import analyse_directory

root_dir = '/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/scripts/pipeline/'
files_dir = os.path.join(root_dir, 'results_10')
output_dir = os.path.join(root_dir, 'output')

analyse_directory(files_dir, output_dir)
