import os
from parameters import Parameters
import analyses
import visualisation

root_dir = '/home/rferon/work/code/radseq_analyses_pipeline/'
files_dir = os.path.join(root_dir, 'results_10')
output_dir = os.path.join(root_dir, 'output')
results_dir = os.path.join(root_dir, 'results')

error_threshold = 0.1

parameters = Parameters(root_dir=root_dir,
                        files_dir=files_dir,
                        output_dir=output_dir,
                        results_dir=results_dir,
                        error_threshold=error_threshold)

analyses.run(parameters)
# visualisation.run(parameters)
