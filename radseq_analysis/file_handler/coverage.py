import statistics


def get_coverage(coverage_file_path):

    '''
    Extract coverage information from a coverage file.
    Coverage file has the following structure:
    Barcode | Individual | Total reads | Retained reads
    Output structure:
    { Individual : Correction }
    '''

    coverage_data = {}
    temp = {}
    coverage_file = open(coverage_file_path)

    for line in coverage_file:
        fields = line[:-1].split('\t')
        temp[fields[1]] = int(fields[3])

    mean_coverage = statistics.mean(temp.values())
    for individual, reads in temp.items():
        coverage_data[individual] = reads / mean_coverage

    return coverage_data
