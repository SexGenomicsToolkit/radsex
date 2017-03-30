import os
import re
from collections import OrderedDict
from utils import list_files, get_info, next_batch, list_species
from analyses import analyse

HIGH = 'high'
LOW = 'low'
HALF_L = 'half_low'
HALF_H = 'half_high'

root_dir = '/home/rferon/work/analyses/multi_species/rad_seq/denovo_map'
haplotype_data_dir = os.path.join(root_dir, 'data')
results_dir = os.path.join(root_dir, 'results')

batch = next_batch(results_dir)
print(' - Batch number: ' + batch)
batch_dir = os.path.join(results_dir, 'batch_' + batch)
os.mkdir(batch_dir)

haplotype_files = list_files(haplotype_data_dir)
species_list = list_species(haplotype_files)
os.mkdir(os.path.join(batch_dir, 'species'))

for species in species_list:
    os.mkdir(os.path.join(batch_dir, 'species', species))

os.mkdir(os.path.join(batch_dir, 'frequencies'))
os.mkdir(os.path.join(batch_dir, 'sex_specific'))
os.mkdir(os.path.join(batch_dir, 'sex_polymorphic'))
os.mkdir(os.path.join(batch_dir, 'environmental'))

margins = OrderedDict([(LOW, 0.05),
                      (HIGH, 0.95),
                      (HALF_L, 0.4),
                      (HALF_H, 0.6)])

margins_file = open(os.path.join(batch_dir, 'margins.txt'), 'w')
for margin, value in margins.items():
    margins_file.write(margin + '\t' + str(value) + '\n')

print(' - Parsing files: ')

for f in haplotype_files:
    infos = get_info(f)
    print('   > ' + infos['species'] + '_' + infos['parameter'])
    species_output = os.path.join(batch_dir, 'species', infos['species'])
    analyse(f, species_output, margins)

for species in species_list:
    species_output = os.path.join(batch_dir, 'species', species)
    output_files = list_files(species_output)
    for o in output_files:
        reg = re.search(r'(\D)_(\D+)_(\d+)_(\D+).([tsvx]{3})', o)
        if reg and len(reg.groups()) == 5:
            o_type = reg.group(4)
            os.system('ln -sf  ' + o + ' ' + os.path.join(batch_dir, o_type))
