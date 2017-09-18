

def plink_ped_file(output_file_root, popmap, haplotypes):

    '''
    Output a plink ped file based on haplotypes distribution
    '''

    output_file_path = output_file_root + '.ped'

    with open(output_file_path, 'w') as output_file:
        for individual, data in haplotypes.items():
            if popmap[individual] == 'M':
                output_file.write('1' + '\t')
                output_file.write(individual + '\t')
                output_file.write('0' + '\t')
                output_file.write('0' + '\t')
                output_file.write('1' + '\t')
                output_file.write('1' + '\t')
            else:
                output_file.write('2' + '\t')
                output_file.write(individual + '\t')
                output_file.write('0' + '\t')
                output_file.write('0' + '\t')
                output_file.write('2' + '\t')
                output_file.write('2' + '\t')
            output_file.write('\t'.join([data[haplotype] + '\t' + data[haplotype] for
                                         haplotype in sorted(data.keys(), key=int)]) + '\n')
