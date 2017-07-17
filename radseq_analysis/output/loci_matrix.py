

def loci_matrix(output_file_path, matrix_data):

    '''
    Output information about loci sex-linkage as a matrix with number of males
    in columns and number of females in row (from top left corner)
    '''

    with open(output_file_path, 'w') as output_file:
        for i in range(len(matrix_data)):
            for j in range(len(matrix_data[0])):
                output_file.write(str(matrix_data[i][j]))
                if j < len(matrix_data[0]) - 1:
                    output_file.write('\t')
                else:
                    output_file.write('\n')
