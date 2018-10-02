RADSex usage details
====================


General
-------

The RADSex software presents the general command-line interface:

``radsex <command> [options]``

**Available commands**

=======  ===========
Command  Description
=======  ===========
process  Compute a table of coverage from a set of demultiplexed reads
distrib  Compute the distribution of sequences between sexes
subset   Extract a subset of the coverage table
signif   Extract sequences significantly associated with sex
loci     Recreate polymorphic loci from a subset of coverage table
mapping  Map a subset of sequences (coverage table or fasta) to a reference genome and output sex-association metrics for each mapped sequence
freq     Compute sequence frequencies for the population
=======  ===========


process
-------

**Command**

::

    radsex process --input-dir input_dir_path --output-file output_file_path [ --threads n_threads --min-coverage min_cov ]

The ``process`` command generates a table showing the coverage of each marker in each individual of the dataset. The output is a tabulated file, where each line contains the ID, sequence, and coverage for each individual of a marker.*

**Options**

==================  ===========
Option              Description
==================  ===========
``--input-dir``     Path to a folder containing demultiplexed reads
``--output-file``   Path to the output file
``--threads``       Number of threads to use (default: 1)
``--min-coverage``  Minimum coverage to consider a sequence in an individual (default: 1)
==================  ===========

**Sample output**

::

    ID    Sequence    individual_1    individual_2    individual_3    individual_4    individual_5
    0     TGCA..TATT             0              15              24              17              21
    1     TGCA..GACC            20              18               3              26               4
    2     TGCA..ATCG             2               1               5              16               0
    3     TGCA..CCGA            14              29              23               2              19



distrib
-------

**Command**

::

    radsex distrib --input-file input_file_path --output-file output_file_path --popmap-file popmap_file_path [ --min-coverage min_cov --output-matrix ]

The ``distrib`` command generates a table containing the number of sequences present with coverage higher than min_cov in *M* males and *F* females for every combination of number of males *M* and number of females *F*.

**Options**

===================  ===========
Option               Description
===================  ===========
``--input-file``     Path to a folder containing demultiplexed reads
``--output-file``    Path to the output file
``--popmap-file``    Path to a popmap file indicating the sex of each individual
``--min-coverage``   Minimum coverage to consider a sequence in an individual (default: 1)
``--output-matrix``  If true, outputs the results as a matrix with males in columns and females in rows instead of a table (default: 0)
===================  ===========

**Sample output**

::

    Males    Females    Sequences      P      Signif
      0         1           7          1      False
      0         2           3       0.39      False
      0         3           1       0.10      False
      1         0           6          1      False
      1         1           5          1      False
      1         2           1          1      False
      1         3           2       0.39      False
      2         0           3       0.39      False
      2         1           8          1      False
      2         2           4          1      False
      2         3           2          1      False
      3         0           4       0.10      False
      3         1           7       0.39      False
      3         2           6          1      False
      3         3           9          1      False

subset
------

**Command**

::

    radsex subset --input-file input_file_path --output-file output_file_path --popmap-file popmap_file_path [ --output-format output_format --min-coverage min_cov --min-males min_males --min-females min_females --max-males max_males --max-females max_females --min-individuals min_individuals --max-individuals max_individuals]

The ``subset`` command filters the coverage table to only export sequences present in any combination of M males and F females, with min_males ≤ M ≤ max_males, min_females ≤ F ≤ max_females, and min_individuals ≤ M + F ≤ max_individuals.

**Options**

=====================  ===========
Option                 Description
=====================  ===========
``--input-file``       Path to an coverage table obtained with ``process``
``--output-file``      Path to the output file
``--popmap-file``      Path to a popmap file indicating the sex of each individual
``--output-format``    Output format, either "table" or "fasta" (default: "table")
``--min-coverage``     Minimum coverage to consider a sequence present in an individual (default: 1)
``--min-males``        Minimum number of males with a retained sequence (default: 0)
``--min-females``      Minimum number of females with a retained sequence (default: 0)
``--max-males``        Maximum number of males with a retained sequence (default: all)
``--max-females``      Maximum number of females with a retained sequence (default: all)
``--min-individuals``  Minimum number of individuals with a retained sequence (default: 1)
``--max-individuals``  Maximum number of individuals with a retained sequence (default: all)
=====================  ===========

**Sample output**

* Table format :

::

    ID    Sequence    individual_1    individual_2    individual_3    individual_4    individual_5
    15    TGCA..TATT             0              15              24              17              21
    27    TGCA..GACC            20              18               3              26               4
    43    TGCA..ATCG             2               1               5              16               0
    86    TGCA..CCGA            14              29              23               2              19

* FASTA format :

::

    >15_5_0_cov:5
    TGCA..TATT
    >15_5_1_cov:5
    TGCA..GACC
    >15_5_1_cov:5
    TGCA..ATCG
    >15_5_0_cov:5
    TGCA..CCGA

signif
------

map
----

freq
----
