# TODO: verifications


def load_popmap(popmap_file_path, global_parameters):

    popmap_file = open(popmap_file_path)

    popmap = {line.split('\t')[0]: line.split('\t')[1][:-1] for
              line in popmap_file if line[:-1]}

    global_parameters.popmap = popmap

    global_parameters.n_males = list(popmap.values()).count('M')
    global_parameters.n_females = list(popmap.values()).count('F')
