# RadSex

## Overview

The RADSex pipeline implements several functions for the analysis of RAD-Sequencing data with focus on sex. This pipeline was developed for the PhyloSex project, which investigates sex determining factors in a wide range of fish species.

The RADSex pipeline was developed by Romain Feron and Yann Guiguen while working at INRA, Rennes, France.

## Requirements

- A C++11 compliant compiler (GCC >= 4.8.1, Clang >= 3.3)
- The zlib library (which should be installed on linux by default)

## Installation

- Clone: `git clone git@github.com:INRA-LPGP/RadSex.git`
- Alternative: Download the archive and unzip it
- Go to the RadSex directory (`cd RadSex`)
- Run `make`
- The compiled `radsex` binary is located in `RadSex/bin/`

## Usage

### General

`radsex <command> [options]`

**Available commands** :

Command            | Description
------------------ | ------------
`process`    | Compute a matrix of coverage from a set of demultiplexed reads files
`distrib` | Compute the distribution of sequences between sexes
`subset` | Extract a subset of the coverage matrix
`signif` | Extract sequences significantly associated with sex
`loci` | Recreate polymorphic loci from a subset of coverage matrix
`mapping` | Map a subset of sequences (coverage table or fasta) to a reference genome and output sex-association metrics for each mapped sequence
`freq` | Compute sequence frequencies for the population

### process

`radsex process -d input_dir_path -o output_file_path [ -t n_threads -c min_cov ]`

*Generates a matrix of coverage for all individuals and all sequences. The output is a tabulated file, where each line contains the ID, sequence and coverage for each individual of a marker.*

**Options** :

Option | Full name | Description
--- | --- | ---
`-d` | `input_dir_path` | Path to a folder containing demultiplexed reads |
`-o` | `output_file_path` | Path to the output file |
`-t` | `n_threads` | Number of threads to use (default: 1) |
`-c` | `min_cov` | Minimum coverage to consider a marker in an individual (default: 1) |

### distrib

`radsex distrib -f input_file_path -o output_file_path -p popmap_file_path [ -c min_cov --output-matrix ]`

*Generates a table which contains the number of sequences present with coverage higher than min_cov and the probability of association with sex for every combination of number of males and number of females.*

**Options** :

Option | Full name | Description
--- | --- | ---
`-f` | `input_file_path` | Path to an coverage matrix obtained with `process` |
`-o` | `output_file_path` | Path to the output file |
`-p` | `popmap_file_path` | Path to a popmap file indicating the sex of each individual |
`-c` | `min_cov` | Minimum coverage to consider a sequence present in an individual (default: 1) |

### Subset

`radsex subset -f input_file_path -o output_file_path -p popmap_file_path [ -c min_cov --min-males min_males --min-females min_females --max-males max_males --max-females max_females --min-individuals min_individuals --max-individuals max_individuals]`

*Filters the coverage matrix to only export sequences present in any combination of M males and F females, with min_males ≤ M ≤ max_males, min_females ≤ F ≤ max_females, and min_individuals ≤ M + F ≤ max_individuals*

**Options** :

Option | Full name | Description
--- | --- | ---
`-f` | `input_file_path` | Path to an coverage matrix obtained with `process` |
`-o` | `output_file_path` | Path to the output file |
`-p` | `popmap_file_path` | Path to a popmap file indicating the sex of each individual |
`-c` | `min_cov` | Minimum coverage to consider a sequence in an individual (default: 1) |
`--min-males` | `min_males` | Minimum number of males with the sequence |
`--min-females` | `min_females` | Minimum number of females with the sequence |
`--max-males` | `max_males` | Maximum number of males with the sequence |
`--max-females` | `max_females` | Maximum number of females with the sequence |
`--max-individuals` | `max_individuals` | Maximum number of individuals with the sequence |
`--max-individuals` | `max_individuals` | Maximum number of individuals with the sequence |

### LICENSE

Copyright (C) 2018 Romain Feron and INRA LPGP

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/
