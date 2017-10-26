# TODO: sort output by marker and individuals ?


def markers(output_file_path, markers_data, individual_names):

    '''
    Output markers data in the following format:
    TODO
    '''

    output_file = open(output_file_path, 'w')
    output_file.write('Marker_ID' + '\t')
    output_file.write('\t'.join(individual_names) + '\n')

    for marker_id, marker in markers_data.items():
        output_file.write(marker_id + '\t')
        output_file.write('\t'.join([str(marker.haplotypes[marker_id].individuals[name]) for
                                     name in individual_names]) + '\n')
