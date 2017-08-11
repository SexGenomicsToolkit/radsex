import os
from collections import defaultdict

'''
BIG WARNING
IF MULTIPLE INSTANCES ARE RUNNING CONCURRENTLY, FILE NAMES WILL MESS IT UP
NEED TO FIND A GOOD WAY TO RESOLVE THIS
'''


def create_temp_seq_file(sequences, species):

    temp_sequences_name = species + '_sequences.temp'

    with open(temp_sequences_name, 'w') as o:
        for locus_id, locus in sequences.items():
            o.write('>' +
                    str(locus_id) + '_' +
                    str(locus.n_males) + '_' +
                    str(locus.n_females) + '\n')
            o.write(locus.sequence + '\n')


def create_temp_catalog_file(catalog_data, species):

    temp_db_name = species + '_catalog.temp'

    with open(temp_db_name, 'w') as o:
        for locus_id, sequence in catalog_data.items():
            o.write('>' + str(locus_id) + '\n')
            o.write(sequence + '\n')


# Todo: better system calls
def create_blast_db(species):

    temp_db_name = species + '_catalog.temp'
    temp_db_logs_name = species + '_blast_db_logs.temp'
    cmd = ('makeblastdb -in ' + temp_db_name + ' -dbtype nucl > ' +
           temp_db_logs_name)
    os.system(cmd)


def run_blast(species):

    temp_sequences_name = species + '_sequences.temp'
    temp_db_name = species + '_catalog.temp'
    temp_blast_logs_name = species + '_blast_logs.temp'
    temp_blast_results_name = species + '_blast_results.temp'

    cmd = ('blastn+ -db ' + temp_db_name +
           ' -query ' + temp_sequences_name +
           ' -outfmt "6 qseqid sseqid length nident mismatch gaps qseq sseq"' +
           ' -out ' + temp_blast_results_name +
           ' > ' + temp_blast_logs_name)
    os.system(cmd)


def cleanup_temp_files(species):

    temp_sequences_name = species + '_sequences.temp'
    temp_db_name = species + '_catalog.temp'
    temp_blast_logs_name = species + '_blast_logs.temp'
    temp_blast_results_name = species + '_blast_results.temp'
    temp_db_logs_name = species + '_blast_db_logs.temp'

    os.remove(temp_db_name)
    os.remove(temp_db_name + '.nhr')
    os.remove(temp_db_name + '.nin')
    os.remove(temp_db_name + '.nsq')
    os.remove(temp_sequences_name)
    os.remove(temp_blast_results_name)
    os.remove(temp_blast_logs_name)
    os.remove(temp_db_logs_name)


def filter_blast_output(species):

    stacks = defaultdict(lambda: dict())
    temp_blast_results_name = species + '_blast_results.temp'
    results = open(temp_blast_results_name)
    for line in results:
        fields = line[:-1].split('\t')
        if fields[2] == '94' and int(fields[3]) >= 90:
            stacks[fields[0]][fields[1]] = [fields[3], fields[4], fields[5]]

    return stacks


def get_matching_sequences(sequences, catalog_data, species):
    create_temp_seq_file(sequences, species)
    create_temp_catalog_file(catalog_data, species)
    create_blast_db(species)
    run_blast(species)
    stacks = filter_blast_output(species)
    cleanup_temp_files(species)
    return stacks
