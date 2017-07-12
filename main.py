import os
from radseq_analyses_pipeline import analyse_directory

root_dir = '/home/rferon/work/data/sample_denovo_results/'
files_dir = os.path.join(root_dir, 'm_5')
analyses_dir = os.path.join(root_dir, 'analyses')

# Maximum proportion of individuals deviating from the population
positions_list = [(34, 0), (33, 0)]

# Path to population map
popmap_path = os.path.join(root_dir, 'ameirus_melas_popmap.csv')

analyse_directory(files_dir, analyses_dir,
                  positions_list, popmap_path)
