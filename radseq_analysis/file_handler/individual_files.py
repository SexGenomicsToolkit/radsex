import gzip
from collections import defaultdict


def get_individual_sequences(individual_file_path, loci_to_extract=None):

    # Open file, read 2nd line, extract individual number and reset to 2nd line
    individual_file = gzip.open(individual_file_path, 'rt')
    individual_file.readline()
    individual_number = individual_file.readline().split('\t')[1]
    individual_file.close()
    individual_file = gzip.open(individual_file_path, 'rt')
    individual_file.readline()

    individual_data = defaultdict(int)

    for line in individual_file:

        tabs = line.split('\t')
        individual_locus_id = tabs[2]

        if (not loci_to_extract or individual_locus_id in
                loci_to_extract[individual_number].keys()):

            catalog_id = loci_to_extract[individual_number][individual_locus_id]

            sequence_name = tabs[6]
            if tabs[7] != '':
                sequence_name += '_' + tabs[7]

            if sequence_name in ['model', 'consensus']:
                pass
            else:
                individual_data[catalog_id] += 1

    return individual_data
