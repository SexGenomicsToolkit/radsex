import os


def frequencies(input_file_path, output_file_path):

    scripts_d = ''.join(os.path.split(os.path.realpath(__file__))[:-1])
    cmd = ('Rscript ' + os.path.join(scripts_d, 'r_scripts', 'frequencies.R') +
           ' ' + input_file_path +
           ' ' + output_file_path)
    os.system(cmd)
