from radseq_analysis import file_handler
from radseq_analysis import output
from radseq_analysis.modules import blast
from radseq_analysis.shared import Stack
import os
from collections import defaultdict


def make_stacks(blast_results, consensus):

    stacks = {}

    for stack_id, haplotypes in blast_results.items():
        s = Stack()
        s.sequence = consensus[stack_id.split('_')[0]]
        for haplotype_id, data in haplotypes.items():
            s.add_haplotype(haplotype_id, consensus[haplotype_id], data)
        stacks[stack_id] = s

    return stacks


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


def fill_individual_data(stacks, individual_data, coverage):

    for stack_id, stack in stacks.items():
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
            stacks[stack_id].haplotypes[haplotype_id].individuals = temp


def analysis(parameters):

    print(' - Loading extracted sequences and catalog data ...')
    coverage = None
    if parameters.coverage_file_path:
        coverage = file_handler.get_coverage(parameters.coverage_file_path)
    sequences = file_handler.get_sequences(parameters.sequences_file_path)
    consensus, correspondance = file_handler.get_info_from_catalog(parameters.catalog_file_path,
                                                                   consensus=True,
                                                                   correspondance=True)
    print(' - Finding similar sequences with blast ...')
    blast_results = blast.get_matching_sequences(sequences, consensus, parameters.species)
    print(' - Creating stacks ...')
    stacks = make_stacks(blast_results, consensus)
    individual_data = get_individual_data(parameters.individual_files_paths, correspondance)
    print(' - Merging individual data in stacks ...')
    fill_individual_data(stacks, individual_data, coverage)
    output.stacks(parameters.output_file_path, stacks, parameters.popmap)
