from collections import defaultdict
import os
from .commons import *
from ..loci_data import Locus

'''
The haplotype file regroups haplotype information for each individual and
each locus. We use it as a base to find sex-linked loci.
'''


def get_haplotypes(file_path, global_parameters):

    '''
    Input: path to a haplotype file (batch_X.haplotypes.tsv)
    Output:
        1) individual haplotypes sorted by sex for each catalog haplotype
           --> { Haplotype_ID:  { SEX: tuple(haplotypes) } }
        2) number of males / females --> { SEX: number }
    '''

    haplotype_file = open(file_path)

    # Haplotypes file has a first line containing the name of each individual
    sex = [global_parameters.popmap[tab] for
           tab in haplotype_file.readline()[:-1].split('\t')[2:]]

    numbers = {MALES: sex.count('M'), FEMALES: sex.count('F')}

    # Data structures
    haplotypes = defaultdict(lambda: dict())

    # Sort individual haplotypes by sex for each catalog haplotype
    for line in haplotype_file:
        tabs = line[:-1].split('\t')
        locus_id = tabs[0]
        haplotypes[locus_id] = {MALES: tuple(seq for i, seq in enumerate(tabs[2:]) if
                                sex[i] is 'M'),
                                FEMALES: tuple(seq for i, seq in enumerate(tabs[2:]) if
                                sex[i] is 'F')}

    return haplotypes, numbers


def fill_matrix(tags, loci_matrix):

    for tag, tag_numbers in tags.items():
        if tag != '-':
            loci_matrix[tag_numbers[FEMALES]][tag_numbers[MALES]] += 1


def check_sex_variable(tags, margins):

    for tag, tag_numbers in tags.items():
        sex_variable = check_tag(tag, tag_numbers, MALES, margins)
        if not sex_variable:
            sex_variable = check_tag(tag, tag_numbers, FEMALES, margins)
        if sex_variable:
            break

    return sex_variable


def check_tag(tag, numbers, main, margins):

    MAIN = main
    if MAIN == FEMALES:
        OPPOSITE = MALES
    else:
        OPPOSITE = FEMALES

    sex_variable = False
    if numbers[MAIN] > margins[MAIN][HIGH]:
        if numbers[OPPOSITE] < margins[OPPOSITE][LOW]:
            sex_variable = True

    return sex_variable


def sex_haplotypes(haplotypes):

    max_m = 0
    max_f = 0
    hap_m = None
    hap_f = None

    for haplotype, count in haplotypes.items():
        if count[MALES] > max_m:
            hap_m = haplotype
            max_m = count[MALES]
        if count[FEMALES] > max_f:
            hap_f = haplotype
            max_f = count[FEMALES]

    return {MALES: (hap_m, max_m), FEMALES: (hap_f, max_f)}


def filter(haplotypes, numbers, results_dir, error_threshold):

    cst_m = int(numbers[MALES] * error_threshold)
    cst_f = int(numbers[FEMALES] * error_threshold)
    margins = {MALES: {HIGH: numbers[MALES] - cst_m, LOW: cst_m},
               FEMALES: {HIGH: numbers[FEMALES] - cst_f, LOW: cst_f}}

    loci_of_interest = {}

    matrix_file = open(os.path.join(results_dir, 'loci_matrix.tsv'), 'w')

    loci_matrix = [[0 for x in range(numbers[MALES] + 1)] for
                   x in range(numbers[FEMALES] + 1)]

    for locus_id, haplotype in haplotypes.items():

        males = tuple((i, m) for i, m in enumerate(haplotype[MALES]))
        females = tuple((i, m) for i, m in enumerate(haplotype[FEMALES]))

        tags = defaultdict(lambda: {MALES: 0, FEMALES: 0})

        for tag in males:
            tags[tag[1]][MALES] += 1
        for tag in females:
            tags[tag[1]][FEMALES] += 1

        fill_matrix(tags, loci_matrix)

        sex_variable = check_sex_variable(tags, margins)

        if sex_variable:
            locus = Locus()
            locus.haplotypes = tags
            locus.individual_haplotypes = haplotype
            locus.n_males = numbers[MALES]
            locus.n_females = numbers[FEMALES]
            locus.haplotypes = sex_haplotypes(tags)
            locus.outliers[MALES] = {i for i, m in enumerate(haplotype[MALES]) if m != locus.haplotypes[MALES][0]}
            locus.outliers[FEMALES] = {i for i, m in enumerate(haplotype[FEMALES]) if m != locus.haplotypes[FEMALES][0]}
            loci_of_interest[locus_id] = locus

    for i in range(len(loci_matrix)):
        for j in range(len(loci_matrix[0])):
            matrix_file.write(str(loci_matrix[i][j]))
            if j < len(loci_matrix[0]) - 1:
                matrix_file.write('\t')
            else:
                matrix_file.write('\n')

    return loci_of_interest


def analyse(file_path, global_parameters):

    loci_of_interest = []

    print('    # Parsing haplotype file ...')
    haplotypes, numbers = get_haplotypes(file_path, global_parameters)
    print('    # Filtering sex variable loci ...')
    loci_of_interest = filter(haplotypes, global_parameters.output_dir, numbers, global_parameters.error_threshold)
    # filter(haplotypes, numbers, global_parameters.output_dir)
    print('    > Sex variable loci extracted')

    return loci_of_interest
