from .commons import *


def sex_linked_haplotypes(output_file_path, loci_data):

    '''
    Output information on sex-linked haplotypes in the following format:
    TODO
    '''

    with open(output_file_path, 'w') as output_file:
        output_file.write('Locus' + '\t' + 'Male_haplotype' + '\t' + 'Male_haplotype_number' + '\t' +
                          'Female_haplotype' + '\t' + 'Female_haplotype_number' +
                          '\t' + 'Males' + '\t' + 'Females' + '\t' + 'Male_outliers' +
                          '\t' + 'Female_outliers' + '\t' + 'Consensus' + '\n')
        for locus, data in loci_data.items():
            output_file.write(str(locus) + '\t' +
                              str(data.haplotypes[MALES][0]) + '\t' +
                              str(data.haplotypes[MALES][1]) + '\t' +
                              str(data.haplotypes[FEMALES][0]) + '\t' +
                              str(data.haplotypes[FEMALES][1]) + '\t' +
                              str(data.n_males) + '\t' +
                              str(data.n_females) + '\t' +
                              '-'.join(str(i) for i in data.outliers[MALES]) + '\t' +
                              '-'.join(str(i) for i in data.outliers[FEMALES]) + '\t' +
                              str(data.consensus) + '\n')
