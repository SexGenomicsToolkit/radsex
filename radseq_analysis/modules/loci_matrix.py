from radseq_analysis.shared.commons import *
from radseq_analysis import file_handler
from radseq_analysis import output


def fill_loci_matrix(parameters):

    print(' - Loading haplotypes from file ...')
    numbers = file_handler.get_haplotypes(parameters, haplotypes=False, numbers=True)

    loci_matrix = [[0 for x in range(int(parameters.n_males) + 1)] for
                   y in range(int(parameters.n_females) + 1)]

    print(' - Generating loci matrix ...')

    for locus_id, data in numbers.items():
        for tag, tag_numbers in data.items():
            if tag != '-':
                loci_matrix[tag_numbers[FEMALES]][tag_numbers[MALES]] += 1

    print(' - Generating output ...')

    return loci_matrix


def analysis(parameters):

    loci_matrix = fill_loci_matrix(parameters)
    output.loci_matrix(parameters.output_file_path, loci_matrix)
