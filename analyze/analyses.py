import os
from collections import defaultdict
from utils import get_info, parse_header, clean_split


MALES = 'males'
FEMALES = 'females'
HIGH = 'high'
LOW = 'low'
HALF_L = 'half_low'
HALF_H = 'half_high'


def write_output_file(data, file, correspondance):

    # Refactorize this shit
    file.write('#MALES\n')
    for locus in data[MALES]:
        file.write(locus + '\t' + correspondance[locus] + '\n')
    file.write('\n\n')
    file.write('#FEMALES\n')
    for locus in data[FEMALES]:
        file.write(locus + '\t' + correspondance[locus] + '\n')


def analyse(file_path, catalog_path, output_dir, margin_ratios):

    infos = get_info(file_path)
    file = open(file_path)

    o_frequencies_p = os.path.join(output_dir,
                                   infos['file_base'] + '_frequencies.tsv')
    o_specific_p = os.path.join(output_dir,
                                infos['file_base'] + '_sex_specific.txt')
    o_polymorphic_p = os.path.join(output_dir,
                                   infos['file_base'] + '_sex_polymorphic.txt')
    o_environmental_p = os.path.join(output_dir,
                                     infos['file_base'] + '_environmental.txt')

    o_frequencies_f = open(o_frequencies_p, 'w')
    o_specific_f = open(o_specific_p, 'w')
    o_polymorphic_f = open(o_polymorphic_p, 'w')
    o_environmental_f = open(o_environmental_p, 'w')

    # Parse header to extract the names, sexes, and column number of samples
    header = file.readline()
    names, sexes, pos = parse_header(header)
    total_males = len(sexes[MALES])
    total_females = len(sexes[FEMALES])

    # Calculate margins for each sex based on margin ratios defined by user
    # Encapsulate this shit
    margins = {MALES:
               {LOW: round(int(margin_ratios[LOW] * total_males)),
                HIGH: round(int(margin_ratios[HIGH] * total_males)),
                HALF_L: round(int(margin_ratios[HALF_L] * total_males)),
                HALF_H: round(int(margin_ratios[HALF_H] * total_males))},
               FEMALES:
               {LOW: round(int(margin_ratios[LOW] * total_females)),
                HIGH: round(int(margin_ratios[HIGH] * total_females)),
                HALF_L: round(int(margin_ratios[HALF_L] * total_females)),
                HALF_H: round(int(margin_ratios[HALF_H] * total_females))}
               }

    sex_specific = {MALES: set(), FEMALES: set()}
    sex_polymorphic = {MALES: set(), FEMALES: set()}
    alleles_frequencies = {}
    environmental = {MALES: set(), FEMALES: set()}

    for line in file:

        tabs = clean_split(line)
        locus_id = tabs[0]
        samples = tabs[pos[0]:pos[1]]
        sex_count = {MALES: 0, FEMALES: 0}
        sex_poly = {MALES: defaultdict(int), FEMALES: defaultdict(int)}
        alleles_frequencies[locus_id] = tabs[1]

        # Encapsulate this shit
        for i, sample in enumerate(samples):
            if sample != '-':
                if names[i] in sexes[MALES]:
                    sex_count[MALES] += 1
                    sex_poly[MALES][sample] += 1
                elif names[i] in sexes[FEMALES]:
                    sex_count[FEMALES] += 1
                    sex_poly[FEMALES][sample] += 1

        # Refactorise this shit
        if sex_count[MALES] >= margins[MALES][HIGH]:
            if sex_count[FEMALES] <= margins[FEMALES][LOW]:
                sex_specific[MALES].add(locus_id)
            for site, count in sex_poly[MALES].items():
                if (site not in sex_poly[FEMALES].keys() or
                        sex_poly[FEMALES][site] <= margins[FEMALES][LOW]):
                        if count >= margins[MALES][HIGH]:
                            sex_polymorphic[MALES].add(locus_id)
                        elif (count >= margins[MALES][HALF_L] and
                              count <= margins[MALES][HALF_H]):
                            environmental[MALES].add(locus_id)

        if sex_count[FEMALES] >= margins[FEMALES][HIGH]:
            if sex_count[MALES] <= margins[MALES][LOW]:
                sex_specific[FEMALES].add(locus_id)
            for site, count in sex_poly[FEMALES].items():
                if (site not in sex_poly[MALES].keys() or
                        sex_poly[MALES][site] <= margins[MALES][LOW]):
                        if count >= margins[FEMALES][HIGH]:
                            sex_polymorphic[FEMALES].add(locus_id)
                        elif (count >= margins[FEMALES][HALF_L] and
                              count <= margins[FEMALES][HALF_H]):
                            environmental[FEMALES].add(locus_id)

    for locus, frequency in alleles_frequencies.items():
        o_frequencies_f.write(locus + '\t' + frequency + '\n')

    catalog = open(catalog_path)
    catalog.readline()
    correspondance = {}
    for line in catalog:
        tabs = clean_split(line)
        locus_id = tabs[2]
        if (locus_id in sex_specific[MALES] or
                locus_id in sex_specific[FEMALES] or
                locus_id in sex_polymorphic[MALES] or
                locus_id in sex_polymorphic[FEMALES] or
                locus_id in environmental[MALES] or
                locus_id in environmental[FEMALES]):
            correspondance[locus_id] = tabs[8]

    write_output_file(sex_specific, o_specific_f, correspondance)
    write_output_file(sex_polymorphic, o_polymorphic_f, correspondance)
    write_output_file(environmental, o_environmental_f, correspondance)
