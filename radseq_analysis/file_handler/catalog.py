from collections import defaultdict
from radseq_analysis.file_handler.file_open import open_all


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

    catalog = open_all(catalog_path)
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


def get_haplotypes(parameters, individuals_only=False):

    '''
    Extract haplotypes information, sorted by sex, from a catalog file
    Input:
        - path to a catalog file (batch_X.catalog.tsv)
    Output:
        - for each locus, haplotype for each individual
        { Locus ID: { sequence: sequence, individuals: { individual_id: individual_locus_id } }}
    '''

    catalog = open_all(parameters.catalog_file_path)
    catalog.readline()

    haplotypes_data = {}

    for line in catalog:
        tabs = line.split('\t')
        locus_id = tabs[2]
        indiv_ids = tabs[8].split(',')
        haplotypes_data[locus_id] = {}
        if not individuals_only:
            haplotypes_data[locus_id]['sequence'] = tabs[9]
            haplotypes_data[locus_id]['individuals'] = {}
            for individual in indiv_ids:
                haplotypes_data[locus_id]['individuals'][individual.split('_')[0]] = individual.split('_')[1]
                haplotypes_data[locus_id]['n_males'] = len({i for i in haplotypes_data[locus_id]['individuals'].keys() if
                                                            parameters.popmap[parameters.order[i]] is 'M'})
                haplotypes_data[locus_id]['n_females'] = len({i for i in haplotypes_data[locus_id]['individuals'].keys() if
                                                              parameters.popmap[parameters.order[i]] is 'F'})
        else:
            haplotypes_data[locus_id]['individuals'] = [individual.split('_')[0] for individual in indiv_ids]
            haplotypes_data[locus_id]['n_males'] = len({i for i in haplotypes_data[locus_id]['individuals'] if
                                                        parameters.popmap[parameters.order[i]] is 'M'})
            haplotypes_data[locus_id]['n_females'] = len({i for i in haplotypes_data[locus_id]['individuals'] if
                                                          parameters.popmap[parameters.order[i]] is 'F'})

    catalog.close()

    return haplotypes_data
