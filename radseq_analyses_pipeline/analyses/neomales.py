import os


def analyse(haplotype_path, global_parameters):

    bin_path = os.path.join(global_parameters.root_dir, 'include',
                            'radseq_bootstrap', 'bin', 'radseq_bootstrap')
    cmd = bin_path
    cmd += ' -t ' + str(global_parameters.n_threads)
    cmd += ' -o ' + global_parameters.neomales_file
    cmd += ' -f ' + haplotype_path
    os.system(cmd)
