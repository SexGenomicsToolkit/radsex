

def load_positions_list(positions_file_path, global_parameters):

    positions = []

    with open(positions_file_path) as positions_file:
        for line in positions_file:
            try:
                fields = line[:-1].split('\t')
                positions.append(tuple([int(fields[0]), int(fields[1])]))
            except IndexError:
                print('Error: cannot read positions file')
                exit(1)

    global_parameters.positions_list = positions
