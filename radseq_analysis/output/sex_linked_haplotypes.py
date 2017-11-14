

def locus_in_position_list(locus, positions_list):

    for position in positions_list:
        if (locus['n_females'] == position[1] and
                locus['n_males'] == position[0]):
            return True
    return False


def sex_linked_haplotypes(parameters, haplotypes):

    '''
    Output information on sex-linked haplotypes in the following format:
    TODO
    '''

    with open(parameters.output_file_path, 'w') as output_file:

        output_file.write('Locus' + '\t' +
                          'Males' + '\t' +
                          'Females' + '\t' +
                          'Sequence' + '\n')

        for locus, data in haplotypes.items():
            if locus_in_position_list(data, parameters.positions_list):
                output_file.write(str(locus) + '\t' +
                                  str(data['n_males']) + '\t' +
                                  str(data['n_females']) + '\t' +
                                  data['sequence'] + '\n')
