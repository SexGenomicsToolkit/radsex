from radseq_analysis import file_handler
import os
from collections import defaultdict

'''
BIG WARNING
IF MULTIPLE INSTANCES ARE RUNNING CONCURRENTLY, FILE NAMES WILL MESS IT UP
NEED TO FIND A GOOD WAY TO RESOLVE THIS
'''


def create_temp_seq_file(sequences):

    with open('sequences.temp', 'w') as o:
        for locus_id, locus in sequences.items():
            o.write('>' +
                    str(locus_id) + '_' +
                    str(locus.n_males) + '_' +
                    str(locus.n_females) + '\n')
            o.write(locus.sequence + '\n')


def create_temp_catalog_file(sequences):

    with open('catalog.temp', 'w') as o:
        for locus_id, sequence in sequences.items():
            o.write('>' + str(locus_id) + '\n')
            o.write(sequence + '\n')


# Todo: better system calls
def create_blast_db():

    cmd = 'makeblastdb -in catalog.temp -dbtype nucl'
    os.system(cmd)


def run_blast():

    cmd = ('blastn -db catalog.temp' +
           ' -query sequences.temp' +
           ' -outfmt "6 qseqid sseqid length nident mismatch gaps qseq sseq"' +
           ' > blast_results.temp')
    os.system(cmd)


def cleanup_temp_files():

    os.remove('catalog.temp')
    os.remove('catalog.temp.nhr')
    os.remove('catalog.temp.nin')
    os.remove('catalog.temp.nsq')
    os.remove('sequences.temp')
    os.remove('blast_results.temp')


def filter_blast_output():

    stacks = defaultdict(lambda: dict())
    results = open('blast_results.temp')
    for line in results:
        fields = line[:-1].split('\t')
        if fields[2] == '94' and int(fields[3]) >= 90:
            stacks[fields[0]][fields[1]] = [fields[3], fields[4], fields[5]]

    return stacks


def analysis(sequences_file_path, catalog_file_path, global_parameters):

    sequences = file_handler.get_sequences(sequences_file_path)

    catalog_data = file_handler.get_info_from_catalog(catalog_file_path)

    create_temp_seq_file(sequences)
    create_temp_catalog_file(catalog_data)
    create_blast_db()
    run_blast()
    stacks = filter_blast_output()
    # cleanup_temp_files()
    for stack, data in stacks.items():
        print(stack)
        for name, sequence in data.items():
            print(name, sequence)
