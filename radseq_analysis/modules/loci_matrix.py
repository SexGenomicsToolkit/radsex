from radseq_analysis import file_handler
from radseq_analysis import output


def fill_loci_matrix(parameters):

    print(' - Loading haplotypes from file ...')
    haplotypes = file_handler.get_haplotypes(parameters, individuals_only=True)

    loci_matrix = [[0 for x in range(int(parameters.n_males) + 1)] for
                   y in range(int(parameters.n_females) + 1)]

    print(' - Generating loci matrix ...')

    for locus_id, data in haplotypes.items():
        loci_matrix[data['n_females']][data['n_males']] += 1

    print(' - Generating output ...')

    return loci_matrix


def analysis(parameters):

    loci_matrix = fill_loci_matrix(parameters)
    output.loci_matrix(parameters.output_file_path, loci_matrix)
