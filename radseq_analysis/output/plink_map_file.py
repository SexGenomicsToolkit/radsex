

def plink_map_file(output_file_root, haplotypes, n_males, n_females):

    '''
    Output a fake plink map file with all loci on chromosome 1 separated by 100 bp
    '''

    output_file_path = output_file_root + '.map'

    with open(output_file_path, 'w') as output_file:
        for i, haplotype in enumerate(sorted(haplotypes.keys(), key=int)):
            id_n = (haplotype + '_' +
                    str(haplotypes[haplotype]['consensus']['males']) + '_' +
                    str(haplotypes[haplotype]['consensus']['females']))
            output_file.write('\t'.join(['1', id_n, '0', str(100 * i)]) + '\n')
