import os
import gzip
from collections import defaultdict
from utils import clean_split, list_files


"""
Optimisation ideas:
- While(True) loop in individual files until all loci have been found
- Check for unnecessary split() and rstrip() - only do it when line needs to be processed.
- Check other parts of the code for unnecessary complex procedures
- Check data structures too.
"""


class File_infos:

    def __init__(self, path):
        self.path = path
        self.base = (os.path.split(path)[-1]).split(os.extsep)[0]
        self.tabs = self.base.split('_')
        self.par = self.tabs[-1]
        self.species = '_'.join(self.tabs[:1])
        self.species_short = self.tabs[0][0] + '_' + self.tabs[1]
        self.id = self.tabs[2]
        self.sex = self.id[0]
        self.number = self.id[1:]


def init_dict(spec_tags):
    spec_loci = dict()
    for k, v in spec_tags.items():
        for i, j in v.items():
            if j not in spec_loci.keys():
                spec_loci[j] = {}
    return spec_loci


def output_files(infos, output_dir, suffix):
    male_p = infos['file_base'] + '_male_' + suffix + '.txt'
    female_p = infos['file_base'] + '_female_' + suffix + '.txt'
    male_f = open(os.path.join(output_dir, male_p), 'w')
    female_f = open(os.path.join(output_dir, female_p), 'w')

    return male_f, female_f


def load_tags(file):

    f = open(file)
    male = True
    male_tags = defaultdict(lambda: defaultdict(str))
    female_tags = defaultdict(lambda: defaultdict(str))
    for line in f:
        if "#MALES" in line:
            male = True
        elif "#FEMALES" in line:
            male = False
        elif line != '\n':
            tabs = clean_split(line)
            catalog_locus = tabs[0]
            for i in tabs[1].split(','):
                individual = i.split('_')[0]
                individual_locus = i.split('_')[1]
                if male:
                    male_tags[individual][individual_locus] = catalog_locus
                else:
                    female_tags[individual][individual_locus] = catalog_locus
    return male_tags, female_tags


def find_tag_files(infos, data_dir):
    species = {'a_melas': 'ameirus_melas',
               'a_gigas': 'arapaima_gigas'}
    print(' - Finding individual tags files')
    tags_dir = os.path.join(data_dir, species[infos['species_short']])
    all_files = list_files(tags_dir, 'gz')
    tag_files = set()
    for t in all_files:
        base = (os.path.split(t)[-1]).split(os.extsep)[0]
        par = base.split('_')[-1]
        if par == infos['parameter'] and 'batch' not in base:
            tag_files.add(t)

    return tag_files


def show_loci(loci_dict):

    for k, v in loci_dict.items():
        print('Locus: ', k)
        for individual, data in v.items():
            print('ID: ', individual)
            print('Consensus: ', data['consensus'])
            for allele_id, allele_data in data['alleles'].items():
                print('Allele: ', allele_id)
                print('Sequence: ', allele_data['sequence'])
                print('Coverage: ', str(allele_data['coverage']))
            print('\n')
        print('\n\n')


def export_loci(loci_dict, file):

    for k, v in loci_dict.items():
        file.write('Locus: ' + k + '\n')
        for individual, data in v.items():
            file.write('ID: ' + individual + '\n')
            file.write('Consensus' + '\t' + data['consensus'] + '\n')
            for allele_id, allele_data in data['alleles'].items():
                file.write(allele_id + '\t')
                file.write(str(allele_data['coverage']) + '\t')
                file.write(allele_data['sequence'] + '\n')
            file.write('\n')
        file.write('\n\n')


def extract_info(tabs, locus, id_n, d):
    if id_n not in d[locus].keys():
        d[locus][id_n] = {'consensus': '',
                          'alleles': {}}
    seq_id = tabs[6] + tabs[7]
    sequence = tabs[9]
    if seq_id == 'consensus':
        d[locus][id_n]['consensus'] = sequence
    elif seq_id != 'model':
        if seq_id not in d[locus][id_n]['alleles'].keys():
            d[locus][id_n]['alleles'][seq_id] = {'sequence': sequence,
                                                 'coverage': 1}
        else:
            d[locus][id_n]['alleles'][seq_id]['coverage'] += 1


def process_file(file_infos, spec_dict, poly_dict, env_dict, spec_tags, poly_tags, env_tags):
    g = gzip.open(file_infos.path, 'rt')
    g.readline()
    for line in g:
        tabs = clean_split(line)
        id_n = tabs[1]
        locus = tabs[2]
        if locus in spec_tags[id_n].keys():
            extract_info(tabs, spec_tags[id_n][locus], file_infos.id, spec_dict)
        if locus in poly_tags[id_n].keys():
            extract_info(tabs, poly_tags[id_n][locus], file_infos.id, poly_dict)
        if locus in env_tags[id_n].keys():
            extract_info(tabs, env_tags[id_n][locus], file_infos.id, env_dict)


def sex_variable(file_spec, file_poly, file_env, data_dir, output_dir):

    base = os.path.splitext(os.path.split(file_spec)[-1])[0]
    path = '/'.join(os.path.split(file_spec)[:-1])
    tabs = base.split('_')

    infos = {'species': tabs[0] + '_' + tabs[1],
             'species_short': tabs[0][0] + '_' + tabs[1],
             'parameter': tabs[2],
             'file_base': '_'.join(base.split('_')[:3]),
             'path': path}

    print(' - Loading sex-specific tags')
    male_spec_tags, female_spec_tags = load_tags(file_spec)
    print(' - Loading sex-polymorphic tags')
    male_poly_tags, female_poly_tags = load_tags(file_poly)
    print(' - Loading sex-polymorphic tags')
    male_env_tags, female_env_tags = load_tags(file_env)

    tag_files = find_tag_files(infos, data_dir)

    male_spec_loci = init_dict(male_spec_tags)
    female_spec_loci = init_dict(female_spec_tags)
    male_poly_loci = init_dict(male_poly_tags)
    female_poly_loci = init_dict(female_poly_tags)
    male_env_loci = init_dict(male_env_tags)
    female_env_loci = init_dict(female_env_tags)

    print(' - Extracting data from individual tags files')
    n_files = str(len(tag_files))
    for i, t in enumerate(tag_files):
        file_infos = File_infos(t)
        if file_infos.sex == 'M':
            print('        - File ' + str(i + 1) + '/' + n_files + ' (Male) : ' + file_infos.base)
            process_file(file_infos, male_spec_loci, male_poly_loci, male_env_loci,
                         male_spec_tags, male_poly_tags, male_env_tags)
        elif file_infos.sex == 'F':
            process_file(file_infos, female_spec_loci, female_poly_loci, female_env_loci,
                         female_spec_tags, female_poly_tags, female_env_tags)
            print('        - File ' + str(i + 1) + '/' + n_files + ' (Female) : ' + file_infos.base)
        else:
            pass

    print(' - Exporting results')
    male_spec_f, female_spec_f = output_files(infos, output_dir, 'spec')
    male_poly_f, female_poly_f = output_files(infos, output_dir, 'poly')
    male_env_f, female_env_f = output_files(infos, output_dir, 'env')

    export_loci(male_spec_loci, male_spec_f)
    export_loci(female_spec_loci, female_spec_f)
    export_loci(male_poly_loci, male_poly_f)
    export_loci(female_poly_loci, female_poly_f)
    export_loci(male_env_loci, male_env_f)
    export_loci(female_env_loci, female_env_f)
