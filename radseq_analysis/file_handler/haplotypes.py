from collections import defaultdict
from radseq_analysis.shared.commons import *


def get_haplotypes(haplotypes_file_path, global_parameters):

    '''
    Extract haplotypes information, sorted by sex, from a haplotypes file
    Input:
        - path to a haplotypes file (batch_X.haplotypes.tsv)
    Output:
        - for each locus, haplotype for each individual
        { Locus ID: { Individual : Haplotype }}
        - for each locus, male and female count of each haplotype:
        { Locus ID:  { Haplotype: { Males: N, Females: M } } }
    '''

    haplotypes_file = open(haplotypes_file_path)

    line = haplotypes_file.readline()
    names = line[:-1].split('\t')[2:]

    haplotypes = dict()

    # Sort individual haplotypes by sex for each catalog haplotype
    for line in haplotypes_file:
        tabs = line[:-1].split('\t')
        locus_id = tabs[0]
        haplotypes[locus_id] = {names[i]: seq for i, seq in enumerate(tabs[2:])}

    haplotypes_numbers = dict()

    for locus_id, data in haplotypes.items():

        tags = defaultdict(lambda: {MALES: 0, FEMALES: 0})

        for individual, haplotype in data.items():

            if individual in global_parameters.popmap.keys():

                if global_parameters.popmap[individual] is 'M':
                    tags[haplotype][MALES] += 1
                elif global_parameters.popmap[individual] is 'F':
                    tags[haplotype][FEMALES] += 1

        haplotypes_numbers[locus_id] = tags

    return haplotypes, haplotypes_numbers
