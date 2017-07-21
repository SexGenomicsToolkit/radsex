from radseq_analysis.shared import *
from radseq_analysis.shared import Locus


def get_sequences(sequences_file_path):

    '''
    Extract information from a sequences file (product of haplotypes analysis)
    '''

    sequences = {}

    sequences_file = open(sequences_file_path)
    sequences_file.readline()

    for line in sequences_file:

        fields = line[:-1].split('\t')
        if len(fields) == 6:
            temp_locus = Locus()
            temp_locus.sequence = fields[3]
            temp_locus.n_males = int(fields[1])
            temp_locus.n_females = int(fields[2])
            temp_locus.outliers[MALES] = set(i for i in fields[4].split(',') if
                                             fields[4])
            temp_locus.outliers[FEMALES] = set(i for i in fields[5].split(',') if
                                               fields[5])
            sequences[int(fields[0])] = temp_locus

    return sequences
