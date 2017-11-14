from radseq_analysis import file_handler
from radseq_analysis import output


def fill_loci_matrix(parameters):

    print(' - Loading haplotypes from file ...')
    haplotypes = file_handler.get_haplotypes(parameters)

    loci_matrix = [[0 for x in range(int(parameters.n_males) + 1)] for
                   y in range(int(parameters.n_females) + 1)]

    print(' - Generating loci matrix ...')

    for locus_id, data in haplotypes.items():
        n_males = len({i for i in data['individuals'].keys() if
                       parameters.popmap[parameters.order[i]] is 'M'})
        n_females = len({i for i in data['individuals'].keys() if
                         parameters.popmap[parameters.order[i]] is 'F'})
        loci_matrix[n_females][n_males] += 1

    print(' - Generating output ...')

    return loci_matrix


def analysis(parameters):

    loci_matrix = fill_loci_matrix(parameters)
    output.loci_matrix(parameters.output_file_path, loci_matrix)
