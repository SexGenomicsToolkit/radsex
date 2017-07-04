

def haplotypes(haplotypes_file_path):

    '''
    Extract haplotypes information, sorted by sex, from a haplotypes file
    Input:
        - path to a haplotypes file (batch_X.haplotypes.tsv)
    Output:
        individual haplotypes sorted by sex:
        { Locus ID:  { Individual name : haplotype } }
    '''

    haplotypes_file = open(haplotypes_file_path)

    line = haplotypes_file.readline()
    names = line[:-1].split('\t')[2:]

    haplotypes = dict(lambda: dict())

    # Sort individual haplotypes by sex for each catalog haplotype
    for line in haplotypes_file:
        tabs = line[:-1].split('\t')
        locus_id = tabs[0]
        haplotypes[locus_id] = {names[i]: seq for i, seq in enumerate(tabs[2:])}

    return haplotypes
