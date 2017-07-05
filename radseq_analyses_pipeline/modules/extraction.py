from radseq_analyses_pipeline.shared.commons import *
from radseq_analyses_pipeline import file_handler
from radseq_analyses_pipeline import output


def locus_in_position_list(tags, positions_list):

    for tag, tag_numbers in tags.items():
        for position in positions_list:
            if tag_numbers[FEMALES] == position[1] and tag_numbers[MALES] == position[0]:
                return True

    return False


def filter(haplotypes_file_path, global_parameters):

    print(' - Loading haplotypes file ...')
    haplotypes = file_handler.haplotypes(haplotypes_file_path, global_parameters)

    numbers = {MALES: global_parameters.popmap.values().count('M'),
               FEMALES: global_parameters.popmap.values().count('F')}

    loci_to_extract = {}

    for locus_id, data in haplotypes.items():

        if locus_in_position_list(tags, global_parameters.positions_list):
            locus = Locus()
            locus.haplotypes = tags
            locus.individual_haplotypes = haplotype
            locus.n_males = numbers[MALES]
            locus.n_females = numbers[FEMALES]
            locus.haplotypes = sex_haplotypes(tags)
            locus.outliers[MALES] = {i for i, m in enumerate(haplotype[MALES]) if m != locus.haplotypes[MALES][0]}
            locus.outliers[FEMALES] = {i for i, m in enumerate(haplotype[FEMALES]) if m != locus.haplotypes[FEMALES][0]}
            loci_to_extract[locus_id] = locus

    return loci_to_extract


def analysis(file_path, global_parameters):

    print('    # Filtering sex variable loci ...')
    loci_to_extract = filter(haplotypes, global_parameters)

    output.sex_linked_haplotypes(global_parameters.haplotypes_output_file,
                                 loci_to_extract)

    return loci_to_extract
