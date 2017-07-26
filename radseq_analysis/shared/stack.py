from collections import defaultdict


class Haplotype:

    def __init__(self, sequence=None, data=None):
        self.sequence = sequence
        self.individuals = defaultdict(int)
        if data:
            self.matches = data[0]
            self.mismatches = data[1]
            self.gaps = data[2]
        else:
            self.matches = 0
            self.mismatches = 0
            self.gaps = 0


class Stack:

    def __init__(self):
        self.sequence = None
        self.haplotypes = {}

    def add_haplotype(self, id_n, sequence=None, data=None):
        self.haplotypes[id_n] = Haplotype(sequence, data)
