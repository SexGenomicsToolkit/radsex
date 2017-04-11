import os


def frequencies(file, output_dir):

    scripts_d = ''.join(os.path.split(os.path.realpath(__file__))[:-1])
    cmd = ('Rscript ' + os.path.join(scripts_d, 'r_scripts', 'frequencies.R') +
           ' ' + file + ' ' + output_dir)
    os.system(cmd)


def haplotypes(file, output_dir, threshold):

    scripts_d = ''.join(os.path.split(os.path.realpath(__file__))[:-1])
    cmd = ('Rscript ' + os.path.join(scripts_d, 'r_scripts', 'haplotypes.R') +
           ' ' + file + ' ' + output_dir + ' ' + str(threshold))
    os.system(cmd)
