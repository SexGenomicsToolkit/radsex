import gzip
import os
from .commons import *

'''
The individual tags files contain all reads for each locus for an individual.
We use this file to extract sequence information in sex-linked loci, and to
compute coverage data.
'''


def names_correspondances(tags_paths):

    ids = {}

    for f in tags_paths:
        name = os.path.split(f)[-1].split('.')[0]
        g = gzip.open(f)
        g.readline()
        line = g.readline().decode('utf-8')
        ids[line.split('\t')[1]] = name

    return ids


def write_output(loci_data, output_path):

    output_file = open(output_path, 'w')
    output_file.write('Locus' + '\t' + 'Individual' + '\t' + 'Allele' + '\t' +
                      'Sequence' + '\t' + 'Coverage' + '\n')
    for locus_id, locus in loci_data.items():
            for individual, alleles in locus.individual_stacks.items():
                for allele, data in alleles.items():
                    output_file.write(str(locus_id) + '\t' + individual + '\t' + allele + '\t' +
                                      data[SEQUENCE] + '\t' + str(data[COVERAGE]) + '\n')


def process_individual(file_path, loci_to_extract, loci_data, names):

    file = gzip.open(file_path, 'rt')
    file.readline()
    for line in file:
        tabs = line.split('\t')
        individual = tabs[1]
        individual_id = tabs[2]
        if individual_id in loci_to_extract[individual].keys():
            catalog_id = loci_to_extract[individual][individual_id]
            if names[individual] not in loci_data[catalog_id].individual_stacks.keys():
                loci_data[catalog_id].individual_stacks[names[individual]] = {}
            name = tabs[6]
            if tabs[7] != '':
                name += '_' + tabs[7]
            sequence = tabs[9]
            if name == 'model':
                pass
            else:
                if name not in loci_data[catalog_id].individual_stacks[names[individual]].keys():
                    loci_data[catalog_id].individual_stacks[names[individual]][name] = {SEQUENCE: sequence,
                                                                                        COVERAGE: 1}
                else:
                    loci_data[catalog_id].individual_stacks[names[individual]][name][COVERAGE] += 1


def analyse(tags_paths, loci_to_extract, loci_data, global_parameters):

    try:
        from progress.bar import Bar
        bar = True
    except ImportError:
        bar = False

    names = names_correspondances(tags_paths)

    if bar:
        progress_bar = Bar('    # Processing individual files', max=len(tags_paths))

    for i, file_path in enumerate(tags_paths):
        if bar:
            progress_bar.next()
        process_individual(file_path, loci_to_extract, loci_data, names)

    print('\n    # Saving data ...')
    write_output(loci_data, global_parameters.alleles_file)
    print('    > Data extracted ...')

    return loci_data
