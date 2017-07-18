import gzip
from collections import defaultdict


def get_info_from_catalog(catalog_path,
                          loci_list=None,
                          consensus=True,
                          correspondance=False,
                          frequencies=False):

    '''
    Extract consensus sequences and loci ID correspondance for each individual
    from the catalog file.
    Input:
        - path to a catalog file (batch_X.catalog.tsv)
    Output (any combination of the following):
        1) ID correspondance for each locus and each individual:
           { Catalog locus ID:  { Individual name: individual locus ID } }
        2) Consensus sequences:
           { Catalog locus ID: consensus sequence }
        3) Haplotypes frequencies table:
           { Number of individuals with haplotype: count }
    '''

    catalog = gzip.open(catalog_path, 'rt')
    catalog.readline()

    if correspondance:
        correspondance_data = defaultdict(lambda: defaultdict())
    if consensus:
        consensus_data = dict()
    if frequencies:
        frequencies_data = defaultdict(int)

    for line in catalog:
        tabs = line.split('\t')
        locus_id = tabs[2]
        if frequencies:
            # Count each individual only once - sometimes they appear more than once ...
            n_indivs = len({i.split('_')[0] for i in tabs[8].split(',')})
            frequencies_data[n_indivs] += 1
        if consensus or correspondance:
            if (loci_list and locus_id in loci_list) or not loci_list:
                if consensus:
                    consensus_data[locus_id] = tabs[9]
                if correspondance:
                    indiv_ids = tabs[8].split(',')
                    for individual in indiv_ids:
                        temp = individual.split('_')
                        correspondance_data[temp[0]][temp[1]] = locus_id

    catalog.close()

    if consensus and correspondance and frequencies:
        return consensus_data, correspondance_data, frequencies_data
    elif consensus and correspondance:
        return consensus_data, correspondance_data
    elif consensus and frequencies:
        return consensus_data, frequencies_data
    elif correspondance and frequencies:
        return correspondance_data, frequencies_data
    elif consensus:
        return consensus_data
    elif correspondance:
        return correspondance_data
    elif frequencies:
        return frequencies_data
