
class Parameters:

    def __init__(self,
                 files_dir='',
                 positions_list=None,
                 output_individuals=False,
                 output_file_path=None,
                 frequencies_output_file='frequencies.tsv',
                 loci_matrix_output_file='haplotypes_matrix.tsv',
                 haplotypes_output_file='extracted_haplotypes.tsv',
                 individuals_output_file='individual_sequences.tsv',
                 popmap={},
                 n_males=0,
                 n_females=0,
                 species=None
                 ):

        self.files_dir = files_dir
        self.positions_list = positions_list
        self.output_file_path = output_file_path
        self.frequencies_output_file = frequencies_output_file
        self.loci_matrix_output_file = loci_matrix_output_file
        self.haplotypes_output_file = haplotypes_output_file
        self.individuals_output_file = individuals_output_file
        self.popmap = popmap
        self.n_males = n_males
        self.n_females = n_females
        self.species = species
