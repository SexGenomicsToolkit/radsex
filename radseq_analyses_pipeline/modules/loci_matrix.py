from radseq_analyses_pipeline.shared.commons import *
from radseq_analyses_pipeline import file_handler
from radseq_analyses_pipeline import output


def fill_loci_matrix(haplotypes_file_path, global_parameters):

    print(' - Loading haplotypes from file ...')
    haplotypes, numbers = file_handler.get_haplotypes(haplotypes_file_path, global_parameters)

    loci_matrix = [[0 for x in range(int(global_parameters.n_males) + 1)] for
                   y in range(int(global_parameters.n_females) + 1)]

    print(' - Generating loci matrix ...')

    for locus_id, data in numbers.items():
        for tag, tag_numbers in data.items():
            if tag != '-':
                loci_matrix[tag_numbers[FEMALES]][tag_numbers[MALES]] += 1

    print(' - Generating output ...')

    return loci_matrix


def analysis(haplotypes_file_path, global_parameters):

    loci_matrix = fill_loci_matrix(haplotypes_file_path, global_parameters)
    output.loci_matrix(global_parameters.loci_matrix_output_file, loci_matrix)
