from collections import defaultdict
import re
from .utils import clean_split
from .commons import *
from ..loci_data import Locus

'''
The haplotype file regroups haplotype information for each individual and
each locus. We use it as a base to find sex-linked loci.
'''


def parse_header(header):

    '''
    Analyse header line from haplotype file to extract individual columns
    '''

    individual_regex = re.compile(r'(.+)_(.+)_(\D+)(\d+)')
    tabs = clean_split(header)
    columns = {MALES: set(), FEMALES: set()}
    names = []

    for i, tab in enumerate(tabs):
        individuals = individual_regex.search(tab)
        if individuals:
            names.append(tab)
            if individuals.group(3) == 'M':
                columns[MALES].add(i)
            elif individuals.group(3) == 'F':
                columns[FEMALES].add(i)

    return columns, names


def get_haplotypes(file_path):

    '''
    Input: path to a haplotype file (batch_X.haplotypes.tsv)
    Output:
        1) individual haplotypes sorted by sex for each catalog haplotype
           --> { Haplotype_ID:  { SEX: tuple(haplotypes) } }
        2) number of males / females --> { SEX: number }
    '''

    haplotype_file = open(file_path)

    # Haplotypes file has a first line containing the name of each individual
    header = haplotype_file.readline()
    columns, names = parse_header(header)
    numbers = {MALES: len(columns[MALES]), FEMALES: len(columns[FEMALES])}

    # Data structures
    haplotypes = defaultdict(lambda: dict())

    # Sort individual haplotypes by sex for each catalog haplotype
    for line in haplotype_file:
        tabs = clean_split(line)
        locus_id = tabs[0]
        temp = {MALES: tuple(), FEMALES: tuple()}
        temp[MALES] = tuple(seq for i, seq in enumerate(tabs) if i in columns[MALES])
        temp[FEMALES] = tuple(seq for i, seq in enumerate(tabs) if i in columns[FEMALES])
        haplotypes[locus_id] = temp

    return haplotypes, numbers


def check_tag(tag, numbers, main, margins):

    if tag == '-':
        return None

    MAIN = main
    if MAIN == FEMALES:
        OPPOSITE = MALES
    else:
        OPPOSITE = FEMALES

    sex_variable = None
    if numbers[MAIN] > margins[SPEC][MAIN][HIGH]:
        if numbers[OPPOSITE] < margins[SPEC][OPPOSITE][LOW]:
            sex_variable = 'full'

    # elif numbers[MAIN] > margins[POLY][OPPOSITE][LOW] and numbers[MAIN] < margins[POLY][OPPOSITE][HIGH]:
    #     if numbers[OPPOSITE] < margins[SPEC][OPPOSITE][LOW]:
    #         sex_variable = 'half'

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


def filter(haplotypes, numbers, error_threshold):

    cst_m = int(numbers[MALES] * error_threshold)
    cst_f = int(numbers[FEMALES] * error_threshold)
    margins = {SPEC: {MALES: {HIGH: numbers[MALES] - cst_m, LOW: cst_m},
                      FEMALES: {HIGH: numbers[FEMALES] - cst_f, LOW: cst_f}},
               POLY: {MALES: {HIGH: numbers[MALES] / 2 + cst_m, LOW: numbers[MALES] / 2 - cst_m},
                      FEMALES: {HIGH: numbers[FEMALES] / 2 + cst_f, LOW: numbers[FEMALES] / 2 - cst_f}}
               }

    loci_of_interest = {}

    for locus_id, haplotype in haplotypes.items():

        males = tuple((i, m) for i, m in enumerate(haplotype[MALES]))
        females = tuple((i, m) for i, m in enumerate(haplotype[FEMALES]))

        tags = defaultdict(lambda: {MALES: 0, FEMALES: 0})

        for tag in males:
            tags[tag[1]][MALES] += 1
        for tag in females:
            tags[tag[1]][FEMALES] += 1

        sex_variable = None
        for tag, tag_numbers in tags.items():
            sex_variable = check_tag(tag, tag_numbers, MALES, margins)
            if not sex_variable:
                sex_variable = check_tag(tag, tag_numbers, FEMALES, margins)
            if sex_variable:
                break

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

    return loci_of_interest


def analyse(file_path, global_parameters):

    print('    # Parsing haplotype file ...')
    haplotypes, numbers = get_haplotypes(file_path)
    print('    # Filtering sex variable loci ...')
    loci_of_interest = filter(haplotypes, numbers, global_parameters.error_threshold)
    print('    > Sex variable loci extracted')

    return loci_of_interest
