import gzip
from collections import defaultdict
from .commons import *


def analyse(catalog_path):

    catalog = gzip.open(catalog_path, 'rt')
    catalog.readline()
    correspondance = defaultdict(lambda: defaultdict())

    for line in catalog:
        tabs = line.split('\t')
        locus_id = tabs[2]
        indiv_ids = tabs[8].split(',')
        for individual in indiv_ids:
            temp = individual.split('_')
            correspondance[temp[0]][temp[1]] = locus_id
            loci_data[locus_id].consensus = tabs[9]

    return correspondance
