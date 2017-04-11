from collections import defaultdict
import os
import re
from .utils import clean_split
from .commons import *


class Locus:

    def __init__(self, locus_id):
        self.id = locus_id
        self.seq = ''
        self.males = 0
        self.females = 0
        self.outliers = {MALES: set(), FEMALES: set()}


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


def write_output(output_dir, loci_of_interest):

    output = os.path.join(output_dir, 'haplotypes_data.tsv')
    with open(output, 'w') as o:
        o.write('Locus' + '\t' + 'Sequence' + '\t' + 'Males' + '\t' + 'Females' + '\t' + 'Male_outliers' + '\t' + 'Female_outliers' + '\n')
        for locus in loci_of_interest:
            o.write(str(locus.id) + '\t' + str(locus.seq) + '\t' + str(locus.males) + '\t' + str(locus.females) + '\t' +
                    '-'.join(str(i) for i in locus.outliers[MALES]) + '\t' + '-'.join(str(i) for i in locus.outliers[FEMALES]) + '\n')


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


def filter(haplotypes, numbers):

    cst = 3
    margins = {SPEC: {MALES: {HIGH: numbers[MALES] - cst, LOW: cst},
                      FEMALES: {HIGH: numbers[FEMALES] - cst, LOW: cst}},
               POLY: {MALES: {HIGH: numbers[MALES] / 2 + cst, LOW: numbers[MALES] / 2 - cst},
                      FEMALES: {HIGH: numbers[FEMALES] / 2 + cst, LOW: numbers[FEMALES] / 2 - cst}}
               }

    loci_of_interest = set()

    for locus_id, haplotype in haplotypes.items():

        males = tuple((i, m) for i, m in enumerate(haplotype[MALES]) if m != '-')
        females = tuple((i, m) for i, m in enumerate(haplotype[FEMALES]) if m != '-')

        tags = defaultdict(lambda: {MALES: 0, FEMALES: 0})

        for tag in males:
            tags[tag[1]][MALES] += 1
        for tag in females:
            tags[tag[1]][FEMALES] += 1

        for tag, numbers in tags.items():

            main = MALES
            opposite = FEMALES

            if numbers[main] > margins[SPEC][main][HIGH]:
                if numbers[opposite] < margins[SPEC][opposite][LOW]:
                    locus = Locus(locus_id)
                    locus.seq = tag
                    locus.males = numbers[MALES]
                    locus.females = numbers[FEMALES]
                    locus.outliers[main] = tuple(i for i, m in enumerate(haplotype[main]) if m != locus.seq)
                    locus.outliers[opposite] = tuple(i for i, m in enumerate(haplotype[opposite]) if m == locus.seq)
                    loci_of_interest.add(locus)

            elif numbers[main] > margins[POLY][opposite][LOW] and numbers[main] < margins[POLY][opposite][HIGH]:
                if numbers[opposite] < margins[SPEC][opposite][LOW]:
                    locus = Locus(locus_id)
                    locus.seq = tag
                    locus.males = numbers[MALES]
                    locus.females = numbers[FEMALES]
                    locus.outliers[main] = tuple(i for i, m in enumerate(haplotype[main]) if m == locus.seq)
                    locus.outliers[opposite] = tuple(i for i, m in enumerate(haplotype[opposite]) if m == locus.seq)
                    loci_of_interest.add(locus)

            main = FEMALES
            opposite = MALES

            if numbers[main] > margins[SPEC][main][HIGH]:
                if numbers[opposite] < margins[SPEC][opposite][LOW]:
                    locus = Locus(locus_id)
                    locus.seq = tag
                    locus.males = numbers[MALES]
                    locus.females = numbers[FEMALES]
                    locus.outliers[main] = tuple(i for i, m in enumerate(haplotype[main]) if m != locus.seq)
                    locus.outliers[opposite] = tuple(i for i, m in enumerate(haplotype[opposite]) if m == locus.seq)
                    loci_of_interest.add(locus)

            elif numbers[main] > margins[POLY][opposite][LOW] and numbers[main] < margins[POLY][opposite][HIGH]:
                if numbers[opposite] < margins[SPEC][opposite][LOW]:
                    locus = Locus(locus_id)
                    locus.seq = tag
                    locus.males = numbers[MALES]
                    locus.females = numbers[FEMALES]
                    locus.outliers[main] = tuple(i for i, m in enumerate(haplotype[main]) if m == locus.seq)
                    locus.outliers[opposite] = tuple(i for i, m in enumerate(haplotype[opposite]) if m == locus.seq)
                    loci_of_interest.add(locus)

    return loci_of_interest


def analyse(file_path, output_dir):

    print('    # Parsing haplotype file ...')
    haplotypes, numbers = get_haplotypes(file_path)
    print('    # Filtering sex variable loci ...')
    loci_of_interest = filter(haplotypes, numbers)
    print('    > Sex variable loci extracted')
    write_output(output_dir, loci_of_interest)

    return loci_of_interest
