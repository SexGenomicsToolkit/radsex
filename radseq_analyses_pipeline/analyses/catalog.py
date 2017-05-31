import gzip
from collections import defaultdict
from .commons import *

'''
The catalog file gives correspondances between locus ID in haplotype file and
locus ID in individual tags files.
'''


def write_output(output, loci_data):

    output = output
    with open(output, 'w') as o:
        o.write('Locus' + '\t' + 'Male_haplotype' + '\t' + 'Male_haplotype_number' + '\t' +
                'Female_haplotype' + '\t' + 'Female_haplotype_number' +
                '\t' + 'Males' + '\t' + 'Females' + '\t' + 'Male_outliers' +
                '\t' + 'Female_outliers' + '\t' + 'Consensus' + '\n')
        for locus, data in loci_data.items():
            o.write(str(locus) + '\t' +
                    str(data.haplotypes[MALES][0]) + '\t' +
                    str(data.haplotypes[MALES][1]) + '\t' +
                    str(data.haplotypes[FEMALES][0]) + '\t' +
                    str(data.haplotypes[FEMALES][1]) + '\t' +
                    str(data.n_males) + '\t' +
                    str(data.n_females) + '\t' +
                    '-'.join(str(i) for i in data.outliers[MALES]) + '\t' +
                    '-'.join(str(i) for i in data.outliers[FEMALES]) + '\t' +
                    str(data.consensus) + '\n')


def analyse(catalog_path, loci_data, global_parameters):

    # Set of loci catalog ID to extract
    loci_to_extract = {locus for locus, data in loci_data.items()}

    catalog = gzip.open(catalog_path, 'rt')
    catalog.readline()
    correspondance = defaultdict(lambda: defaultdict())
    frequencies = defaultdict(int)

    for line in catalog:
        tabs = line.split('\t')
        locus_id = tabs[2]
        indiv_ids = tabs[8].split(',')
        n_indivs = len({i.split('_')[0] for i in indiv_ids})
        frequencies[n_indivs] += 1
        if locus_id in loci_to_extract:
            for individual in indiv_ids:
                temp = individual.split('_')
                correspondance[temp[0]][temp[1]] = locus_id
                loci_data[locus_id].consensus = tabs[9]

    output = global_parameters.frequencies_file
    with open(output, 'w') as o:
        o.write('Frequency' + '\t' + 'Count' + '\n')
        for frequency, count in frequencies.items():
            o.write(str(frequency) + '\t' + str(count) + '\n')

    write_output(global_parameters.haplotypes_file, loci_data)

    return correspondance
