from radseq_analysis import file_handler
from radseq_analysis import output


def analysis(parameters):

    print(' - Loading haplotypes file ...')
    haplotypes = file_handler.get_haplotypes(parameters, sequence=True)

    print(' - Filtering loci ...')
    output.sex_linked_haplotypes(parameters, haplotypes)
