import os
from radseq_analyses_pipeline import analyse_directory


species = 'ameirus_melas'
replicate = None
m_value = 5

positions_list = [(34, 0), (33, 0)]


root_dir = '/work/bimarazene/work/radseq_all/'
extraction_dir = os.path.join(root_dir, 'analyses', 'extraction')

if replicate:
    files_dir = os.path.join(root_dir, 'results', species, replicate, m_value)
    analyses_dir = os.path.join(extraction_dir, species, replicate, m_value)
else:
    files_dir = os.path.join(root_dir, 'results', species, m_value)
    analyses_dir = os.path.join(extraction_dir, species, m_value)

# Path to population map
popmap_path = os.path.join(root_dir, 'data', 'popmaps', species + '_popmap.csv')

analyse_directory(root_dir, files_dir, analyses_dir,
                  positions_list, popmap_path)
