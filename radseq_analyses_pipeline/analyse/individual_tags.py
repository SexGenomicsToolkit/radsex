import gzip
import os
from .commons import *
from progress.bar import Bar


def names_correspondances(tags_paths):

    ids = {}

    for f in tags_paths:
        name = os.path.split(f)[-1].split('.')[0]
        g = gzip.open(f)
        g.readline()
        line = g.readline().decode('utf-8')
        ids[line.split('\t')[1]] = name

    return ids


def write_output(loci_data, output_path, names):

    output_file = open(output_path, 'w')
    output_file.write('Locus' + '\t' + 'Individual' + '\t' + 'Allele' + '\t' +
                      'Sequence' + '\t' + 'Coverage' + '\n')
    for locus, individuals in loci_data.items():
        for individual, alleles in individuals.items():
            for name, data in alleles.items():
                output_file.write(locus + '\t' + names[individual] + '\t' + name + '\t' +
                                  data[SEQUENCE] + '\t' + str(data[COVERAGE]) + '\n')


def process_individual(file_path, loci_to_extract, loci_data):

    file = gzip.open(file_path, 'rt')
    file.readline()
    for line in file:
        tabs = line.split('\t')
        individual = tabs[1]
        individual_id = tabs[2]
        if individual_id in loci_to_extract[individual].keys():
            catalog_id = loci_to_extract[individual][individual_id]
            name = tabs[6]
            if tabs[7] != '':
                name += '_' + tabs[7]
            sequence = tabs[9]
            if name == 'model':
                pass
            else:
                if name not in loci_data[catalog_id][individual].keys():
                    loci_data[catalog_id][individual][name] = {SEQUENCE: sequence,
                                                               COVERAGE: 1}
                else:
                    loci_data[catalog_id][individual][name][COVERAGE] += 1


def analyse(tags_paths, loci_to_extract, global_parameters):

    loci_data = {}

    names = names_correspondances(tags_paths)

    for individual, corr in loci_to_extract.items():
        for individual_id, catalog_id in corr.items():
            if catalog_id not in loci_data.keys():
                loci_data[catalog_id] = {}
            loci_data[catalog_id][individual] = {}

    progress_bar = Bar('    # Processing individual files', max=len(tags_paths))
    for i, file_path in enumerate(tags_paths):
        progress_bar.next()
        process_individual(file_path, loci_to_extract, loci_data)

    print('\n    # Saving data ...')
    write_output(loci_data, global_parameters.alleles_file, names)
    print('    > Data extracted ...')

    return loci_data
