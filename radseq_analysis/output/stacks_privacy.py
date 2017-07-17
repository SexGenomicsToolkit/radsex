

def stacks_privacy(output_file_path, frequencies_data):

    '''
    Output information about stacks privacy in the following format:
    Number of individuals sharing a stack | Count
    '''

    with open(output_file_path, 'w') as output_file:
        output_file.write('Frequency' + '\t' + 'Count' + '\n')
        for frequency, count in frequencies_data.items():
            output_file.write(str(frequency) + '\t' + str(count) + '\n')
