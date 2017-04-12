from .utils import list_files
from . import haplotypes
from . import catalog
from . import individual_tags


def run(global_parameters):

    # Get all required files in the directory
    files = list_files(global_parameters.files_dir, ('tsv.gz', 'tsv'))

    # Path to catalog tags file (batch_X.catalog.tags.tsv.gz)
    catalog_path = [f for f in files if 'catalog.tags' in f][0]

    # Path to haplotypes file (batch_X.haplotypes.tsv)
    haplotype_path = [f for f in files if 'haplotypes' in f][0]

    # Paths to all individual tags files (individual.tags.tsv.gz)
    tags_paths = [f for f in files if 'tags' in f and 'catalog' not in f]

    print(' - Extracting sex variable loci from haplotypes file ...')
    loci_data = haplotypes.analyse(haplotype_path, global_parameters)

    print(' - Extracting corresponding IDs from catalog file ...')
    loci_to_extract = catalog.analyse(catalog_path, loci_data, global_parameters)

    print(' - Extracting data from individual files...')
    individual_tags.analyse(tags_paths, loci_to_extract, loci_data, global_parameters)
