import os


def haplotypes(input_file, output_file, species_name):

    scripts_d = ''.join(os.path.split(os.path.realpath(__file__))[:-1])
    cmd = ('Rscript ' + os.path.join(scripts_d, 'r_scripts', 'heatmap.R') +
           ' ' + input_file +
           ' ' + output_file +
           ' ' + species_name)
    os.system(cmd)
