import gzip
import os
from collections import defaultdict


def analyse(catalog_path, loci_of_interest, output_dir):

    # Set of loci catalog ID to extract
    loci_to_extract = {locus.id for locus in loci_of_interest}

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

    output = os.path.join(output_dir, 'frequencies_data.tsv')
    with open(output, 'w') as o:
        o.write('Frequency' + '\t' + 'Count' + '\n')
        for frequency, count in frequencies.items():
            o.write(str(frequency) + '\t' + str(count) + '\n')

    return correspondance
