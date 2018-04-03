## RadSex

### Overview

The RADSex pipeline is used to analyze RADSeq data with focus on sex. This pipeline was developed for the PhyloSex project, which investigates sex determining factors in a wide range of fish species.

### Requirements

- A C++11 compliant compiler (GCC >= 4.8.1, Clang >= 3.3)
- The zlib library (should be installed on linux by default)
- *Optional (for visualization)* : R 3.3 or higher with the following packages:
    + readr
    + ggplot2
    + reshape2
    + ggdendro
    + grid
    + gtable
    + base
    + svglite
    + scales

### Installation

- Clone: `git clone git@github.com:INRA-LPGP/RadSex.git`
- Alternative: Download the archive and unzip it
- Go to the RadSex directory (`cd RadSex`)
- Run `make`
- *Optional* : install R packages for visualization with `Rscript install_packages.R`

### Usage


#### General

`radsex <command> [options]`

**Available commands** :

Command            | Description
------------------ | ------------
`process_reads`    | Compute a matrix of coverage from a set of demultiplexed reads files
`sex_distribution` | Calculate a distribution of sequences between sexes
`subset` | Extract a subset of the coverage matrix


<br/>

#### Process reads

`radsex process_reads -d input_dir_path -o output_file_path [ -t n_threads -c min_cov ]`

*Generates a matrix of coverage for all individuals and all sequences. The output is a tabulated file, where each line contains the ID, sequence and coverage for each individual of a marker.*

**Options** :

Option | Full name | Description
--- | --- | ---
`-d` | `input_dir_path` | Path to a folder containing demultiplexed reads |
`-o` | `output_file_path` | Path to the output file |
`-t` | `n_threads` | Number of threads to use (default: 1) |
`-c` | `min_cov` | Minimum coverage to consider a marker in an individual (default: 1) |

<br/>

#### Sex distribution

`radsex sex_distribution -f input_file_path -o output_file_path -p popmap_file_path [ -c min_cov ]`

*Generates a matrix of dimensions (Number of males) x (Number of females). The value at coordinates **(i, j)** corresponds to the number of haplotypes found in precisely **i** males and **j** females.*

**Options** :

Option | Full name | Description
--- | --- | ---
`-f` | `input_file_path` | Path to an input file (result of process_reads) |
`-o` | `output_file_path` | Path to the output file |
`-p` | `popmap_file_path` | Path to a popmap file (indicating the sex of each individual) |
`-c` | `min_cov` | Minimum coverage to consider a marker in an individual (default: 1) |

<br/>

#### Subset

`radsex subset -f input_file_path -o output_file_path -p popmap_file_path [ -c min_cov --min-males min_males --min-females min_females --max-males max_males --max-females max_females ]`

*Filters the coverage matrix to export markers matching the values of min_males, min_females, max_males, and max_females (i.e. markers found in M males with min_males <= M <= max_males and F females with min_females <= F <= max_females)*

**Options** :

Option | Full name | Description
--- | --- | ---
`-f` | `input_file_path` | Path to an input file (result of process_reads) |
`-o` | `output_file_path` | Path to the output file |
`-p` | `popmap_file_path` | Path to a popmap file (indicating the sex of each individual) |
`-c` | `min_cov` | Minimum coverage to consider a marker in an individual (default: 1) |
`--min-males` | `min_males` | Minimum number of males with a marker |
`--min-females` | `min_females` | Minimum number of females with a marker |
`--max-males` | `max_males` | Maximum number of males with a marker |
`--max-females` | `max_females` | Maximum number of females with a marker |

<br/>

**Example output** :

- heatmap :
![Heatmap](./examples/plots/heatmap.png)
- clustering :
![Presence/Absence](./examples/plots/presence_clustering.png)
![Coverage](./examples/plots/coverage_clustering.png)
- frequencies:
![Coverage](./examples/plots/frequencies.png)

### LICENSE

Copyright (C) 2018 Romain Feron and INRA LPGP

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/
