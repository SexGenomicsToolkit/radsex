import os


def clustering(input_file_path, popmap_file_path, output_dir_path):

    scripts_d = ''.join(os.path.split(os.path.realpath(__file__))[:-1])
    cmd = ('Rscript ' + os.path.join(scripts_d, 'r_scripts', 'clustering_species.R') +
           ' ' + input_file_path +
           ' ' + popmap_file_path +
           ' ' + output_dir_path)
    os.system(cmd)
