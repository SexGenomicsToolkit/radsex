from radseq_analysis.shared import *
from radseq_analysis import file_handler
from radseq_analysis import output


def get_majority_haplotypes(data):

    male_majority = None
    female_majority = None

    if data['consensus'][MALES] > data['-'][MALES]:
        male_majority = 'consensus'
    else:
        male_majority = '-'

    if data['consensus'][FEMALES] > data['-'][FEMALES]:
        female_majority = 'consensus'
    else:
        female_majority = '-'

    return male_majority, female_majority


def locus_in_position_list(tags, positions_list):

    # if tags['consensus'][FEMALES] == 0 and tags['consensus'][MALES] == 34:
    #     print(tags)
    if set(tags.keys()).issubset(['consensus', '-']):
        for position in positions_list:
            if (tags['consensus'][FEMALES] == position[1] and
                    tags['consensus'][MALES] == position[0]):
                return True
        return False
    else:
        print(tags)
        return False


def filter(haplotypes_file_path, global_parameters):

    print(' - Loading haplotypes file ...')
    haplotypes, numbers = file_handler.get_haplotypes(haplotypes_file_path, global_parameters)

    loci_to_extract = {}

    print(' - Filtering loci ...')
    for locus_id, data in numbers.items():
        if locus_in_position_list(data, global_parameters.positions_list):
            locus = Locus()
            locus.n_males = data['consensus'][MALES]
            locus.n_females = data['consensus'][FEMALES]
            male_majority, female_majority = get_majority_haplotypes(data)
            locus.outliers[MALES] = set(i for i, v in haplotypes[locus_id].items() if
                                        v != male_majority and global_parameters.popmap[i] is 'M')
            locus.outliers[FEMALES] = set(i for i, v in haplotypes[locus_id].items() if
                                          v != female_majority and global_parameters.popmap[i] is 'F')
            loci_to_extract[locus_id] = locus

    return loci_to_extract


def analysis(haplotypes_file_path, catalog_file_path, global_parameters):

    print('    # Filtering sex variable loci ...')
    loci_to_extract = filter(haplotypes_file_path, global_parameters)
    correspondance, consensus = file_handler.get_info_from_catalog(catalog_file_path, loci_to_extract)

    for locus_id, sequence in consensus.items():
        loci_to_extract[locus_id].sequence = sequence

    output.sex_linked_haplotypes(global_parameters.haplotypes_output_file,
                                 loci_to_extract)

    return loci_to_extract
