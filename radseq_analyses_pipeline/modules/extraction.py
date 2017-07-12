from radseq_analyses_pipeline.shared.commons import *
from radseq_analyses_pipeline import file_handler
from radseq_analyses_pipeline import output


def locus_in_position_list(tags, positions_list):

    if set(tags.keys()) == {'consensus', '-'}:
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
    haplotypes, numbers = file_handler.haplotypes(haplotypes_file_path, global_parameters)

    loci_to_extract = {}

    for locus_id, data in numbers.items():
        if locus_in_position_list(tags, global_parameters.positions_list):
            locus = Locus()
            locus.n_males = tags['consensus'][MALES]
            locus.n_females = tags['consensus'][FEMALES]
            locus.outliers[MALES] = set(i for i, v in haplotypes[locus_id].items() if
                                        v is "-" and global_parameters.popmap[v] is 'M')
            locus.outliers[FEMALES] = set(i for i, v in haplotypes[locus_id].items() if
                                          v is "-" and global_parameters.popmap[v] is 'F')
            loci_to_extract[locus_id] = locus

    return loci_to_extract


def analysis(haplotypes_file_path, catalog_file_path, global_parameters):

    print('    # Filtering sex variable loci ...')
    loci_to_extract = filter(haplotypes_file_path, global_parameters)
    correspondance, consensus = get_info_from_catalog(catalog_file_path, loci_to_extract)

    for locus_id, sequence in consensus.items():
        loci_to_extract[locus_id].sequence = sequence

    output.sex_linked_haplotypes(global_parameters.haplotypes_output_file,
                                 loci_to_extract)

    return loci_to_extract
