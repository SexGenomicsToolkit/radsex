

def stacks(output_file_path, stacks_data, popmap):

    '''
    Output stacks data in the following format:
    TODO
    '''

    output_file = open(output_file_path, 'w')
    output_file.write('Stack_ID' + '\t' + 'Haplotype_ID' + '\t' + 'Sequence' + '\t')
    males = [i for i, s in popmap.items() if s == 'M']
    females = [i for i, s in popmap.items() if s == 'F']

    for m in males:
        output_file.write(m + '\t')
    for i, f in enumerate(females):
        output_file.write(f)
        if i < len(females) - 1:
            output_file.write('\t')
        else:
            output_file.write('\n')

    for stack_id, stack in stacks_data.items():
        for haplotype_id, haplotype in stack.haplotypes.items():
            output_file.write(stack_id + '\t' +
                              haplotype_id + '\t' +
                              haplotype.sequence + '\t' +
                              str(haplotype.matches) + '\t' +
                              str(haplotype.mismatches) + '\t' +
                              str(haplotype.gaps) + '\t')
            for m in males:
                output_file.write(str(haplotype.individuals[m]) + '\t')
            for i, f in enumerate(females):
                output_file.write(str(haplotype.individuals[f]))
                if i < len(females) - 1:
                    output_file.write('\t')
                else:
                    output_file.write('\n')
