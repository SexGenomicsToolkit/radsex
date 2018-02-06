input_file = open('./test.tsv')
header = input_file.readline()
header = header[:-1].split('\t')

min_males = 1
min_females = 1
min_cov = 0

popmap_file = open('./popmap.tsv')
popmap = {line.split('\t')[0]: line[:-1].split('\t')[1] for line in popmap_file if line[:-1]}

n_males = [p for p in popmap.values()].count('M')
n_females = [p for p in popmap.values()].count('F')

males_col = [i for i, h in enumerate(header) if h in popmap.keys() and popmap[h] is 'M']
females_col = [i for i, h in enumerate(header) if h in popmap.keys() and popmap[h] is 'F']

print(n_females, n_males)
output_file = open('./haplotypes.tsv', 'w')
output_file.write('Locus' + '\t' +
                  'Males' + '\t' +
                  'Females' + '\t' +
                  'Sequence' + '\n')

for i, line in enumerate(input_file):

    if i % 1000000 == 0:
        print(str(i))

    temp = line[:-1].split('\t')
    females = len([t for j, t in enumerate(temp) if j in females_col and int(t) > min_cov])
    males = len([t for j, t in enumerate(temp) if j in males_col and int(t) > min_cov])

    if males > min_males and females > min_females:
        output_file.write(str(temp[0]) + '\t' +
                          str(males) + '\t' +
                          str(females) + '\t' +
                          temp[1] + '\n')
