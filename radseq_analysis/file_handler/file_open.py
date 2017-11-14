import gzip


def open_all(file_path):

    if file_path.endswith('.gz'):
        file = gzip.open(file_path, 'rt')
    else:
        file = open(file_path)

    return file
