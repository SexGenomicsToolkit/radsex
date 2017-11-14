import argparse
import sys
import os
from radseq_analysis.modules import analysis


class Parser():

    def __init__(self):
        parser = argparse.ArgumentParser(
            description='Analysis of Stacks denovo map output',
            usage='''python3 radseq_analysis.py <command> [options]

Command:  heatmap\tGenerates a matrix of haplotypes sex distribution
\t  haplotypes\tExtract haplotypes present in a given number of males and females
\t  frequencies\tCalculate haplotypes frequencies distribution in the population
\t  rescue\tRegroup stacks into alleles after analysis
\t  coverage\tExtract individual coverage for a set of markers
\t  visualize\tVisualize analyses results using R
'''
        )
        parser.add_argument('command', help='Command to run', nargs='?')
        args = parser.parse_args(sys.argv[1:2])
        if not args.command:
            print()
            parser.print_usage()
            print()
            exit(1)
        if not hasattr(self, args.command):
            print()
            parser.print_usage()
            print()
            exit(1)
        getattr(self, args.command)()

    def heatmap(self):
        parser = argparse.ArgumentParser(
            description='Generates a matrix of haplotypes sex distribution',
            usage='''python3 radseq_analysis.py heatmap -i input_folder -m popmap [-o output_file]

Options:  -i\t--input-folder\tPath to a folder containing the output of denovo_map
\t  -m\t--popmap\tPath to population map
\t  -o\t--output-file\tPath to output file (default: haplotypes_matrix.tsv)
''')
        parser.add_argument('--input-folder', '-i',
                            help='Path to a folder containing the output of denovo_map')
        parser.add_argument('--popmap', '-m',
                            help='Path to a popmap file')
        parser.add_argument('--output-file', '-o',
                            help='Path to output file', nargs='?',
                            default='haplotypes_matrix.tsv')
        args = parser.parse_args(sys.argv[2:])
        if not args.input_folder or not os.path.isdir(args.input_folder):
            print('\nError: no valid input folder specified\n')
            parser.print_usage()
            print()
            exit(1)
        if not args.popmap or not os.path.isfile(args.popmap):
            print('\nError: no valid popmap file specified\n')
            parser.print_usage()
            print()
            exit(1)
        analysis(input_dir=args.input_folder,
                 popmap_file_path=args.popmap,
                 output_file_path=args.output_file,
                 analysis='heatmap')

    def haplotypes(self):
        parser = argparse.ArgumentParser(
            description='Extract haplotypes for a given set of positions',
            usage='''python3 radseq_analysis.py haplotypes -i input_folder -m popmap -p positions_list [-o output_file]

Options:  -i\t--input-folder\tPath to a folder containing the output of denovo_map
\t  -m\t--popmap\tPath to population map
\t  -p\t--positions\tPath to a file containing the list of positions to etract
\t  -o\t--output-file\tPath to output file (default: extracted_haplotypes.tsv)
''')
        parser.add_argument('--input-folder', '-i',
                            help='Path to a folder containing the output of denovo_map')
        parser.add_argument('--popmap', '-m',
                            help='Path to a popmap file')
        parser.add_argument('--positions', '-p',
                            help='Path to a list of positions')
        parser.add_argument('--output-file', '-o',
                            help='Path to output file', nargs='?',
                            default='extracted_haplotypes.tsv')
        args = parser.parse_args(sys.argv[2:])
        if not args.input_folder or not os.path.isdir(args.input_folder):
            print('\nError: no valid input folder specified\n')
            parser.print_usage()
            print()
            exit(1)
        if not args.popmap or not os.path.isfile(args.popmap):
            print('\nError: no valid popmap file specified\n')
            parser.print_usage()
            print()
            exit(1)
        if not args.positions or not os.path.isfile(args.positions):
            print('\nError: no valid positions file specified\n')
            parser.print_usage()
            print()
            exit(1)
        analysis(input_dir=args.input_folder,
                 popmap_file_path=args.popmap,
                 output_file_path=args.output_file,
                 positions_file_path=args.positions,
                 analysis='haplotypes')

    def frequencies(self):
        parser = argparse.ArgumentParser(
            description='Calculate haplotypes frequencies distribution in the population',
            usage='''python3 radseq_analysis.py frequencies -i input_folder [-o output_file]

Options:  -i\t--input-folder\tPath to a folder containing the output of denovo_map
\t  -o\t--output-file\tPath to output file (default: haplotypes_frequencies.tsv)
''')
        parser.add_argument('--input-folder', '-i',
                            help='Path to a folder containing the output of denovo_map')
        parser.add_argument('--output-file', '-o',
                            help='Path to output file', nargs='?',
                            default='haplotypes_frequencies.tsv')
        args = parser.parse_args(sys.argv[2:])
        if not args.input_folder or not os.path.isdir(args.input_folder):
            print('\nError: no valid input folder specified\n')
            parser.print_usage()
            print()
            exit(1)
        analysis(input_dir=args.input_folder,
                 output_file_path=args.output_file,
                 analysis='frequencies')

    def rescue(self):
        parser = argparse.ArgumentParser(
            description='Regroup stacks into alleles after analysis',
            usage='''python3 radseq_analysis.py rescue -i input_folder -m popmap -s sequences_file [-c coverage_file -o output_file]

Options:  -i\t--input-folder\tPath to a folder containing the output of denovo_map
\t  -s\t--sequences\tPath to sequences file (result of haplotypes analysis)
\t  -m\t--popmap\tPath to population map
\t  -c\t--coverage-file\tPath to a coverage file (result of coverage analysis)
\t  -o\t--output-file\tPath to output file (default: extracted_alleles.tsv)
''')
        parser.add_argument('--input-folder', '-i',
                            help='Path to a folder containing the output of denovo_map')
        parser.add_argument('--sequences', '-s',
                            help='Path to sequences file')
        parser.add_argument('--popmap', '-m',
                            help='Path to a popmap file')
        parser.add_argument('--coverage-file', '-c',
                            help='Path to coverage file', nargs='?')
        parser.add_argument('--output-file', '-o',
                            help='Path to output file', nargs='?',
                            default='extracted_alleles.tsv')
        args = parser.parse_args(sys.argv[2:])
        if not args.input_folder or not os.path.isdir(args.input_folder):
            print('\nError: no valid input folder specified\n')
            parser.print_usage()
            print()
            exit(1)
        if not args.sequences or not os.path.isfile(args.sequences):
            print('\nError: no valid sequences file specified\n')
            parser.print_usage()
            print()
            exit(1)

        analysis(input_dir=args.input_folder,
                 sequences_file_path=args.sequences,
                 popmap_file_path=args.popmap,
                 output_file_path=args.output_file,
                 coverage_file_path=args.coverage_file,
                 analysis='rescue')

    def coverage(self):
        parser = argparse.ArgumentParser(
            description='Extract individual coverage for a set of markers',
            usage='''python3 radseq_analysis.py coverage -i input_folder -a markers_file [-c coverage_file -o output_file]

Options:  -i\t--input-folder\tPath to a folder containing the output of denovo_map
\t  -a\t--markers\tPath to markers file (list of markers to extract)
\t  -c\t--coverage-file\tPath to a coverage file (result of coverage analysis)
\t  -o\t--output-file\tPath to output file (default: markers_coverage.tsv)
''')
        parser.add_argument('--input-folder', '-i',
                            help='Path to a folder containing the output of denovo_map')
        parser.add_argument('--markers', '-a',
                            help='Path to markers file')
        parser.add_argument('--coverage-file', '-c',
                            help='Path to coverage file', nargs='?')
        parser.add_argument('--output-file', '-o',
                            help='Path to output file', nargs='?',
                            default='extracted_alleles.tsv')
        args = parser.parse_args(sys.argv[2:])
        if not args.input_folder or not os.path.isdir(args.input_folder):
            print('\nError: no valid input folder specified\n')
            parser.print_usage()
            print()
            exit(1)
        if not args.markers or not os.path.isfile(args.markers):
            print('\nError: no valid markers file specified\n')
            parser.print_usage()
            print()
            exit(1)

        analysis(input_dir=args.input_folder,
                 markers_file_path=args.markers,
                 output_file_path=args.output_file,
                 coverage_file_path=args.coverage_file,
                 analysis='coverage')

    def visualize(self):
        parser = argparse.ArgumentParser(
            description='Visualize analyses results using R',
            usage='''python3 radseq_analysis.py visualize -i input_file -o output_file -m popmap

Options:  -i\t--input-file\tPath to a file generated by this pipeline (haplotype_matrix, frequencies, ...)
\t  -o\t--output-file\tPath to output file or output directory
\t  -m\t--popmap\tPath to population map
''')
        parser.add_argument('--input-file', '-i',
                            help='Path to a file generated by this pipeline')
        parser.add_argument('--output-file', '-o',
                            help='Path to output file / directory')
        parser.add_argument('--popmap', '-m',
                            help='Path to a popmap file')

        args = parser.parse_args(sys.argv[2:])
        if not args.input_file or not os.path.isfile(args.input_file):
            print('\nError: no valid input file specified\n')
            parser.print_usage()
            print()
            exit(1)
        if not args.output_file:
            print('\nError: no valid output file specified\n')
            parser.print_usage()
            print()
            exit(1)
        if not args.popmap or not os.path.isfile(args.popmap):
            print('\nError: no valid popmap file specified\n')
            parser.print_usage()
            print()
            exit(1)
        analysis(input_file_path=args.input_file,
                 popmap_file_path=args.popmap,
                 output_file_path=args.output_file,
                 analysis='visualize')
