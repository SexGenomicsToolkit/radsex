from radseq_analysis.file_handler.file_open import open_all


def get_individuals_order(log_path, global_parameters):

    '''
    Parse the denovo_map.log to find the order in which individuals were processed,
    which corresponds to the order of the individuals in the catalog.
    '''

    logs = open_all(log_path)

    individuals_order = {}

    line = logs.readline()
    found = False
    while line != 'Depths of Coverage for Processed Samples:\n' and line:
        line = logs.readline()
        if line == 'Depths of Coverage for Processed Samples:\n':
            found = True

    if not found:
        print('Error: could not detect individuals order in the denovo_map log file.\n')
        print('Possible explanations: \n')
        print('\t- you are using an old version of Stacks\n')
        print('\t- Stacks did not run properly and denovo_map.log is incomplete\n')
        print('\t- a newer version of Stacks has changed the log file structure\n')

    order = 1
    while True:
        line = logs.readline()
        if line == '\n':
            break
        individuals_order[str(order)] = line.split(':')[0]
        order += 1

    global_parameters.order = individuals_order
