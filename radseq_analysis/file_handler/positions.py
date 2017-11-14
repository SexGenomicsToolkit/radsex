from radseq_analysis.file_handler.file_open import open_all


def load_positions_list(positions_file_path):

    positions = []

    with open_all(positions_file_path) as positions_file:
        for line in positions_file:
            if line[:-1]:
                try:
                    fields = line[:-1].split('\t')
                    positions.append(tuple([int(fields[0]), int(fields[1])]))
                except IndexError:
                    print('Error: cannot read positions file')
                    exit(1)

    return positions
