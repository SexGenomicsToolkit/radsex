

def get_popmap(file_path):

    file = open(file_path)

    return {line.split('\t')[0]: line.split('\t')[1][:-1] for line in file if
            line[:-1]}
