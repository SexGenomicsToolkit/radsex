import os


class Parameters:

    def __init__(self,
                 files_dir='',
                 results_dir='results',
                 positions_list=None,
                 output_individuals=False,
                 frequencies_output_file='frequencies.tsv',
                 loci_matrix_output_file='loci_matrix.tsv',
                 haplotypes_output_file='sex_linked_haplotypes.tsv',
                 individuals_output_file='individual_sequences.tsv',
                 neomales_output_file='neomales_analysis_results.tsv',
                 neofemales_output_file='neofemales_analysis_results.tsv',
                 n_threads=1,
                 popmap={},
                 n_males=0,
                 n_females=0
                 ):

        self.files_dir = files_dir
        self.results_dir = results_dir
        self.positions_list = positions_list
        self.frequencies_output_file = os.path.join(results_dir, frequencies_output_file)
        self.loci_matrix_output_file = os.path.join(results_dir, loci_matrix_output_file)
        self.haplotypes_output_file = os.path.join(results_dir, haplotypes_output_file)
        self.individuals_output_file = os.path.join(results_dir, individuals_output_file)
        self.neomales_output_file = os.path.join(results_dir, neomales_output_file)
        self.neofemales_output_file = os.path.join(results_dir, neofemales_output_file)
        self.n_threads = n_threads
        self.popmap = popmap
        self.n_males = n_males
        self.n_females = n_females
