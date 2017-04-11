import os


def frequencies(file, output_dir, n_individuals):

    scripts_d = ''.join(os.path.split(os.path.realpath(__file__))[:-1])
    cmd = ('Rscript ' + os.path.join(scripts_d, 'r_scripts', 'ggplot.R') + ' ' +
           file + ' ' + output_dir)
    os.system(cmd)
