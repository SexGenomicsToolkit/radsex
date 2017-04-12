import os
from global_parameters import GlobalParameters
from analyse.analysis import analyse_directory
from visualise.visualisation import visualise

root_dir = '/home/rferon/work/code/radseq_analyses_pipeline/'
files_dir = os.path.join(root_dir, 'results_10')
output_dir = os.path.join(root_dir, 'output')
results_dir = os.path.join(root_dir, 'results')

error_threshold = 0.1

global_parameters = GlobalParameters(root_dir=root_dir,
                                     files_dir=files_dir,
                                     output_dir=output_dir,
                                     results_dir=results_dir,
                                     error_threshold=error_threshold)

analyse_directory(global_parameters)
visualise(global_parameters)
