import os
import sys
from radseq_analyses_pipeline import analyse_directory
from radseq_analyses_pipeline.parameters.positions import get_positions_list


species = 'ameirus_melas'
replicate = None
m_value = 3

positions_file_path = sys.argv[1]
positions_list = get_positions_list(positions_file_path)

root_dir = '/work/bimarazene/radseq_all/'
extraction_dir = os.path.join(root_dir, 'analyses', 'extraction')

if not os.path.isdir(os.path.join(extraction_dir, species)):
    os.mkdir(os.path.join(extraction_dir, species))

if replicate:
    if not os.path.isdir(os.path.join(extraction_dir, species, replicate)):
        os.mkdir(os.path.join(extraction_dir, species, replicate))
    files_dir = os.path.join(root_dir, 'results', species, replicate, 'm_' + str(m_value))
    analyses_dir = os.path.join(extraction_dir, species, replicate, 'm_' + str(m_value))
else:
    files_dir = os.path.join(root_dir, 'results', species, 'm_' + str(m_value))
    analyses_dir = os.path.join(extraction_dir, species, 'm_' + str(m_value))

# Path to population map
popmap_path = os.path.join(root_dir, 'data', 'popmaps', species + '_popmap.csv')

analyse_directory(root_dir, files_dir, analyses_dir,
                  positions_list, popmap_path)
