import re
import os
from radseq_analysis.file_handler import load_popmap
from radseq_analysis.file_handler import load_positions_list
from radseq_analysis.file_handler import get_individuals_order


class Parameters:

    def __init__(self,
                 files_dir='',
                 output_individuals=False,
                 frequencies_output_file='frequencies.tsv',
                 loci_matrix_output_file='haplotypes_matrix.tsv',
                 haplotypes_output_file='extracted_haplotypes.tsv',
                 individuals_output_file='individual_sequences.tsv',
                 positions_list=None,
                 popmap={},
                 order={},
                 n_males=0,
                 n_females=0,
                 species=None,
                 m_value=None,
                 popmap_file_path=None,
                 output_file_path=None,
                 positions_file_path=None,
                 sequences_file_path=None,
                 coverage_file_path=None,
                 markers_file_path=None
                 ):

        self.files_dir = files_dir
        self.positions_list = positions_list
        self.output_file_path = output_file_path
        self.frequencies_output_file = frequencies_output_file
        self.loci_matrix_output_file = loci_matrix_output_file
        self.haplotypes_output_file = haplotypes_output_file
        self.individuals_output_file = individuals_output_file
        self.popmap = popmap
        self.order = order
        self.n_males = n_males
        self.n_females = n_females
        self.species = species
        self.m_value = m_value
        self.haplotypes_file_path = None
        self.catalog_file_path = None
        self.individual_files_paths = []
        self.logs_file_path = None
        self.popmap_file_path = popmap_file_path
        self.output_file_path = output_file_path
        self.positions_file_path = positions_file_path
        self.sequences_file_path = sequences_file_path
        self.coverage_file_path = coverage_file_path
        self.markers_file_path = markers_file_path

    def get_files(self, input_dir):

        if not os.path.isdir(input_dir):
            print('Error: invalid input directory (' + input_dir + ')\n')
            exit()

        files = [f for f in os.listdir(input_dir)]
        for file in files:
            haplotypes_file_re = re.search(r'batch_\d+\.haplotypes\.tsv', file)
            if haplotypes_file_re:
                self.haplotypes_file_path = os.path.join(input_dir, file)
            catalog_file_re = re.search(r'batch_\d+\.catalog\.tags\.tsv', file)
            if catalog_file_re:
                self.catalog_file_path = os.path.join(input_dir, file)
            individual_file_re = re.search(r'(.+)\.tags\.tsv', file)
            if individual_file_re and 'catalog' not in individual_file_re.groups()[0]:
                self.individual_files_paths.append(os.path.join(input_dir, file))

        self.logs_file_path = os.path.join(input_dir, 'denovo_map.log')

        if self.popmap_file_path and os.path.isfile(self.popmap_file_path):
            self.popmap, self.n_males, self.n_females = load_popmap(self.popmap_file_path)
        if self.positions_file_path and os.path.isfile(self.positions_file_path):
            self.positions_list = load_positions_list(self.positions_file_path)
        if self.logs_file_path and os.path.isfile(self.logs_file_path):
            self.order = get_individuals_order(self.logs_file_path)

        self.guess_species()

    def guess_species(self):

        if self.popmap_file_path:
            self.species = os.path.split(self.popmap_file_path)[1]
            self.species = os.path.splitext(self.species)[0]
            if len(self.species.split('_')) > 1:
                self.species = '_'.join(s for s in self.species.split('_')[:-1])
