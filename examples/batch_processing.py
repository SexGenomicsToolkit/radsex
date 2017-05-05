import os
import itertools

analysis = 'denovo_map_par'

species = ['perca_fluviatilis', 'ameirus_melas']

m_values = [5, 10]
n_values = [1, 2, 3, 4]
M_values = [1, 2, 3, 4]

root_dir = '/work/rferon/'
species_dir = os.path.join(root_dir, 'species')
batch_dir = os.path.join(root_dir, 'batch_processing')
shell_dir = os.path.join(batch_dir, 'shell', analysis, 'analyses')
python_dir = os.path.join(batch_dir, 'python', analysis)

qsub_dir = os.path.join(batch_dir, 'qsub')
qsub_all = os.path.join(qsub_dir, analysis + '_analyses.sh')
qsub_file = open(qsub_all, 'w')

for specie, m, n, M in itertools.product(species, m_values, n_values, M_values):

    species_short = specie.split('_')[0][0] + '_' + specie.split('_')[1]

    analysis_path = os.path.join(species_dir, specie, 'analyses', 'rad_seq', analysis)

    results_path = os.path.join(analysis_path, 'results_' +
                                'm_' + str(m) + '_' +
                                'n_' + str(n) + '_' +
                                'M_' + str(M))

    shell_path = os.path.join(shell_dir,
                              specie + '_' +
                              'm_' + str(m) + '_' +
                              'n_' + str(n) + '_' +
                              'M_' + str(M) + '.sh')

    python_path = os.path.join(python_dir,
                               specie + '_' +
                               'm_' + str(m) + '_' +
                               'n_' + str(n) + '_' +
                               'M_' + str(M) + '.py')

    output_dir_base = os.path.join(batch_dir, 'results', analysis, specie)
    if not os.path.isdir(output_dir_base):
        os.mkdir(output_dir_base)
    output_dir = os.path.join(output_dir_base, 'm_' + str(m) + '_' +
                              'n_' + str(n) + '_' +
                              'M_' + str(M))
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)

    python_file = open(python_path, 'w')
    python_file.write("import sys\n\n")
    python_file.write("sys.path.append('/work/rferon/tools/radseq_analyses_pipeline')\n\n")
    python_file.write("from radseq_analyses_pipeline import analyse_directory\n\n")
    python_file.write("analyse_directory('" + results_path + "', '" + output_dir + "')\n")

    shell_file = open(shell_path, 'w')
    shell_file.write('python3 ' + python_path + '\n')

    qsub_file.write('qsub ' + shell_path + '\n')
