from visualize.sex_variable import sex_variable
from analyze.analyses import analyse

spec_p = ('/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/results/' +
          'batch_1/species/a_melas/a_melas_10_sex_specific.txt')
poly_p = ('/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/results/' +
          'batch_1/sex_polymorphic/a_melas_10_sex_polymorphic.txt')
env_p = ('/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/results/' +
         'batch_1/environmental/a_melas_10_environmental.txt')
output_dir = '/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/figures/'
data_dir = '/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/data/tags/'

# frequencies(file_p, output_dir, 50)
sex_variable(spec_p, poly_p, env_p, data_dir, output_dir)

HIGH = 'high'
LOW = 'low'
HALF_L = 'half_low'
HALF_H = 'half_high'

margins = {LOW: 0.05,
           HIGH: 0.95,
           HALF_L: 0.4,
           HALF_H: 0.6}

# analyse('./data/haplotypes/a_melas_denovo_10.tsv', './data/haplotypes/batch_1.catalog.tags.tsv', './results/batch_1/species/a_melas/', margins)
