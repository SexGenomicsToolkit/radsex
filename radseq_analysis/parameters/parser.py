import argparse
import sys
import os
from radseq_analysis.modules import analysis


class Pipeline():

    def __init__(self):
        parser = argparse.ArgumentParser(
            description='Analysis of Stacks denovo map output',
            usage='''python3 radseq_analysis.py <command> [options]

Command:  heatmap\tGenerates a matrix of haplotypes sex distribution
\t  haplotypes\tExtract haplotypes present in a given number of males and females
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
\t  -o\t--output_file\tPath to output file (default: haplotypes_matrix.tsv)
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
\t  -o\t--output_file\tPath to output file (default: extracted_haplotypes.tsv)
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
