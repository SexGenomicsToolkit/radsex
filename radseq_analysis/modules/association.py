from collections import defaultdict
from radseq_analysis import file_handler
from radseq_analysis import output


def analysis(haplotypes_file_path, global_parameters):

    haplotypes, numbers = file_handler.get_haplotypes(haplotypes_file_path, global_parameters,
                                                      haplotypes=True, numbers=True)

    output.plink_map_file(global_parameters.output_file_path,
                          numbers,
                          global_parameters.n_males,
                          global_parameters.n_females)
    individuals = defaultdict(lambda: defaultdict(str))
    for haplotype in sorted(haplotypes.keys(), key=int):
        for individual in haplotypes[haplotype]:
            if haplotypes[haplotype][individual] == 'consensus':
                individuals[individual][haplotype] = 'A'
            else:
                individuals[individual][haplotype] = 'G'
    output.plink_ped_file(global_parameters.output_file_path,
                          global_parameters.popmap,
                          individuals)
