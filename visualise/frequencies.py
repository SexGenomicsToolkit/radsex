import os


def frequencies(file, output_dir, n_individuals):

    scripts_d = ''.join(os.path.split(os.path.realpath(__file__))[:-1])
    cmd = ('Rscript ' + os.path.join(scripts_d, 'ggplot.R') + ' ' + file + ' ' +
           output_dir + ' ' + str(n_individuals))
    os.system(cmd)
