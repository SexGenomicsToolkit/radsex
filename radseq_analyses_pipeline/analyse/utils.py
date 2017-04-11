import os
import re


def clean_split(line, sep='\t'):
    return line.rstrip('\n').split(sep)


def get_info(file):

    base = os.path.splitext(os.path.split(file)[-1])[0]
    path = '/'.join(os.path.split(file)[:-1])
    tabs = base.split('_')
    infos = {'species': tabs[0] + '_' + tabs[1],
             'species_short': tabs[0][0] + '_' + tabs[1],
             'parameter': tabs[3],
             'file_base': '_'.join([tabs[0][0], tabs[1], tabs[3]]),
             'path': path}

    return infos


def list_files(directory, end=('tsv', 'txt')):

    files = [os.path.join(directory, f) for f in os.listdir(directory)
             if os.path.isfile(os.path.join(directory, f)) and
             f.endswith(end)]

    return files


def parse_header(line):

    tabs = clean_split(line)
    names = []
    sexes = {'males': [], 'females': []}
    pos = [-1, -1]
    for i, tab in enumerate(tabs):
        individuals = re.search(r'(.+)_(.+)_(\D+)(\d+)', tab)
        if individuals:
            if pos[0] == -1:
                pos[0] = i
            else:
                pos[1] = i
            names.append(tab)
            if individuals.group(3) == 'M':
                sexes['males'].append(tab)
            elif individuals.group(3) == 'F':
                sexes['females'].append(tab)

    return names, sexes, pos


def next_batch(directory):
    dirs = [d for d in os.listdir(directory)
            if os.path.isdir(os.path.join(directory, d))]
    b = -1
    for d in dirs:
        s = d.split('_')
        if 'batch' in s:
            c = int(s[-1])
            if c > b:
                b = c
    return(str(b + 1))


def list_species(files):
    species = set()
    for f in files:
        species.add(get_info(f)['species'])
    return species
