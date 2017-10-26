from radseq_analysis import file_handler
from radseq_analysis import output
from radseq_analysis.shared import Stack
import os
from collections import defaultdict



def get_individual_names(individual_files_paths):

    return [os.path.split(file)[1].split('.')[0] for file in individual_files_paths]


def initialize_markers(markers_list):

    markers = {}

    for marker_id in markers_list:
        s = Stack()
        s.add_haplotype(marker_id)
        markers[marker_id] = s

    return markers


def get_individual_data(individual_files_paths, correspondance, bar=False):

    individual_data = {}

    try:
        from progress.bar import Bar
        bar = True
    except ImportError:
        bar = False

    if bar:
        progress_bar = Bar(' - Extracting individual data :', max=len(individual_files_paths))
    else:
        print(' - Extracting individual data ...')

    for individual_file_path in individual_files_paths:
        if bar:
            progress_bar.next()
        name = os.path.split(individual_file_path)[1].split('.')[0]
        data = file_handler.get_individual_sequences(individual_file_path,
                                                     correspondance)
        individual_data[name] = data

    if bar:
        print()

    return individual_data


def fill_individual_data(markers, individual_data, coverage):

    for stack_id, stack in markers.items():
        for haplotype_id, haplotype in stack.haplotypes.items():
            temp = defaultdict(int)
            for name, data in individual_data.items():
                if haplotype_id in data.keys():
                    if coverage:
                        temp[name] = int(int(data[haplotype_id]) / coverage[name])
                    else:
                        temp[name] = data[haplotype_id]

                else:
                    temp[name] = 0
            markers[stack_id].haplotypes[haplotype_id].individuals = temp


def analysis(markers_file_path, catalog_file_path,
             individual_files_paths, coverage_file_path, global_parameters):

    print(' - Loading extracted markers and catalog data ...')
    coverage = None
    if coverage_file_path:
        coverage = file_handler.get_coverage(coverage_file_path)
    markers_list = file_handler.get_markers(markers_file_path)
    correspondance = file_handler.get_info_from_catalog(catalog_file_path,
                                                                     loci_list=markers_list,
                                                                     consensus=False,
                                                                     correspondance=True)
    individual_names = get_individual_names(individual_files_paths)
    print(' - Creating stacks ...')
    markers = initialize_markers(markers_list)
    individual_data = get_individual_data(individual_files_paths, correspondance)
    print(' - Merging individual data in stacks ...')
    fill_individual_data(markers, individual_data, coverage)
    output.markers(global_parameters.output_file_path, markers, individual_names)
