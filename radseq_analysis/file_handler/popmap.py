from radseq_analysis.file_handler.file_open import open_all
# TODO: verifications


def load_popmap(popmap_file_path):

    popmap_file = open_all(popmap_file_path)

    popmap = {line.split('\t')[0]: line.split('\t')[1][:-1] for
              line in popmap_file if line[:-1]}

    return [popmap, list(popmap.values()).count('M'), list(popmap.values()).count('F')]
