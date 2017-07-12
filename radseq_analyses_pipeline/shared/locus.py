

class Locus:

    '''
    Object storing information about a locus:
        - Possible haplotypes
        - Haplotype for each individual
        - Number of males and females with this locus
        - Male and female outliers (i.e. individuals who do not follow the
        general model for each population)
        - Stack for each individual: for each allele -> name, sequence, coverage,
        reads matching this allele
    '''

    def __init__(self):

        self.sequence = None
        self.n_males = 0
        self.n_females = 0
        self.outliers = {'males': set(), 'females': set()}
        self.individual_stacks = {}
