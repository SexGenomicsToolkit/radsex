from radseq_analyses_pipeline.shared import *


def individual_sequences(output_file_path, individual_data):

    '''
    Output individual sequence data in the following format:
    TODO
    '''

    output_file = open(output_path, 'w')
    output_file.write('Locus' + '\t' + 'Individual' + '\t' + 'Allele' + '\t' +
                      'Sequence' + '\t' + 'Coverage' + '\n')

    for locus_id, locus in individual_data.items():
            for individual, alleles in locus.individual_stacks.items():
                for allele, data in alleles.items():
                    output_file.write(str(locus_id) + '\t' + individual + '\t' + allele + '\t' +
                                      data[SEQUENCE] + '\t' + str(data[COVERAGE]) + '\n')
