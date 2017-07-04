import gzip
from collections import defaultdict


def catalog(catalog_path):

    '''
    Extract consensus sequences and loci ID correspondance for each individual
    from the catalog file.
    Input:
        - path to a catalog file (batch_X.catalog.tsv)
    Output:
        1) ID correspondance for each locus and each individual:
           { Catalog locus ID:  { Individual name: individual locus ID } }
        2) Consensus sequences:
           { Catalog locus ID: consensus sequence }
    '''

    catalog = gzip.open(catalog_path, 'rt')
    catalog.readline()
    correspondance = defaultdict(lambda: defaultdict())
    consensus = dict()

    for line in catalog:
        tabs = line.split('\t')
        locus_id = tabs[2]
        indiv_ids = tabs[8].split(',')
        for individual in indiv_ids:
            temp = individual.split('_')
            correspondance[temp[0]][temp[1]] = locus_id
        consensus[locus_id] = tabs[9]

    return correspondance, consensus
