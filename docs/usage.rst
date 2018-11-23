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
distrib  Compute the distribution of markers between sexes
subset   Extract a specified subset of the coverage table
signif   Extract markers significantly associated with sex
loci     Recreate polymorphic loci from a subset of coverage table
mapping  Map a subset of markers (coverage table or fasta) to a reference genome and output sex-association metrics for each mapped marker
freq     Compute marker frequencies for the population
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
``--output-file``   Path to the output file (in tsv format)
``--threads``       Number of threads to use (default: 1)
``--min-coverage``  Minimum coverage to consider a marker present in an individual (default: 1)
==================  ===========

**Sample output**

::

    ID    Sequence    individual_1    individual_2    individual_3    individual_4    individual_5
    0     TGCA..TATT             0              15              24              17              21
    1     TGCA..GACC            20              18               3              26               4
    2     TGCA..ATCG             2               1               5              16               0
    3     TGCA..CCGA            14              29              23               2              19


.. note:: Input files are automatically dectected from the input directory based on their extensions. Supported extensions are **.fa**, **.fa.gz**, **.fq**, **.fq.gz**, **.fasta**, **.fasta.gz**, **.fastq**, **.fastq.gz**, **.fna**, and **.fna.gz**. Individual IDs in the output table will be inferred from the input files names after removing the extension. For instance, a file named **individual_1.fastq.gz** will be attributed the ID **individual_1**. Make sure to use the same IDs when creating the population map !


distrib
-------

**Command**

::

    radsex distrib --input-file input_file_path --output-file output_file_path --popmap-file popmap_file_path [ --min-coverage min_cov --output-matrix ]

The ``distrib`` command generates a table containing the number of markers present with coverage higher than min_cov in *M* males and *F* females for every combination of number of males *M* and number of females *F*. For each combination of *M* and *F*, a probability of association with sex is computed using a chi-squared test with Yate's correction for continuity. Significativity is provided in the last column, including Bonferroni correction : markers are significantly associated with sex when p ≤ (0.05 / total number of markers).

**Options**

===================  ===========
Option               Description
===================  ===========
``--input-file``     Path to coverage table generated with ``process``
``--output-file``    Path to the output file (in tsv format)
``--popmap-file``    Path to a popmap file indicating the sex of each individual
``--min-coverage``   Minimum coverage to consider a marker present in an individual (default: 1)
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

The ``subset`` command filters the coverage table to only export markers present in M males and F females, with min_males ≤ M ≤ max_males, min_females ≤ F ≤ max_females, and min_individuals ≤ M + F ≤ max_individuals. Markers can be exported either in table format (same as the output of ``process``) or in fasta format, with marker information contained in the sequence IDs.

**Options**

=====================  ===========
Option                 Description
=====================  ===========
``--input-file``       Path to an coverage table obtained with ``process``
``--output-file``      Path to the output file (in tsv or fasta format)
``--popmap-file``      Path to a popmap file indicating the sex of each individual
``--output-format``    Output format, either "table" or "fasta" (default: "table")
``--min-coverage``     Minimum coverage to consider a marker present in an individual (default: 1)
``--min-males``        Minimum number of males with a retained marker (default: 0)
``--max-males``        Maximum number of males with a retained marker (default: all)
``--min-females``      Minimum number of females with a retained marker (default: 0)
``--max-females``      Maximum number of females with a retained marker (default: all)
``--min-individuals``  Minimum number of individuals with a retained marker (default: 1)
``--max-individuals``  Maximum number of individuals with a retained marker (default: all)
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

In FASTA format, IDs are generated with the following pattern : <marker_ID>_<number_of_males>M_<number_of_females>F_cov:<minimum_coverage>.

::

    >15_5M_0F_cov:5
    TGCA..TATT
    >27_5M_1F_cov:5
    TGCA..GACC
    >43_5M_1F_cov:5
    TGCA..ATCG
    >86_5M_0F_cov:5
    TGCA..CCGA


signif
------

**Command**

::

    radsex signif --input-file input_file_path --output-file output_file_path --popmap-file popmap_file_path [ --output-format output_format --min-coverage min_cov ]

The ``signif`` command filters the coverage table to only export markers significantly associated with sex. The probability of association with sex is computed using a chi-squared test with Yate's correction for continuity. Markers are significantly associated with sex when p ≤ (0.05 / total number of markers), to implement Bonferroni correction. Markers significantly associated with sex can be exported either in table format (same as the output of ``process``) or in fasta format, with marker information contained in the sequence IDs.

**Options**

=====================  ===========
Option                 Description
=====================  ===========
``--input-file``       Path to an coverage table obtained with ``process``
``--output-file``      Path to the output file (in tsv or fasta format)
``--popmap-file``      Path to a popmap file indicating the sex of each individual
``--output-format``    Output format, either "table" or "fasta" (default: "table")
``--min-coverage``     Minimum coverage to consider a marker present in an individual (default: 1)
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

In FASTA format, IDs are generated with the following pattern : <marker_ID>_<number_of_males>M_<number_of_females>F_cov:<minimum_coverage>.

::

    >15_5M_0F_cov:5
    TGCA..TATT
    >27_5M_1F_cov:5
    TGCA..GACC
    >43_5M_1F_cov:5
    TGCA..ATCG
    >86_5M_0F_cov:5
    TGCA..CCGA


loci
----

**Command**

::

    radsex loci --input-file input_file_path --coverage-table coverage_matrix_path --output-file output_file_path  [ --max-distance max_distance --threads n_threads --min-cov min_cov ]

The ``loci`` command attempts to find markers belonging to the same locus for a list of markers (in tsv format) obtained with ``subset`` or ``signif``. For each specified marker, the Levenstein distance to every marker in the original coverage table is computed, and markers with distance shorter than **max_distance** are retained. The output file is a tabulated file where each line corresponds to a marker. The first column gives the ID of the reconstructed polymorphic locus containing this marker, the marker ID from the coverage table is in the second column, and the marker's sequence is in the third column. The last column indicates whether the marker comes from the specified list of markers ("Original") or was recovered from the coverage table ("Recovered").

**Options**

=====================  ===========
Option                 Description
=====================  ===========
``--input-file``       Path to an coverage table obtained with ``subset`` or ``signif``
``--coverage-table``   Path to an coverage table obtained with ``process``
``--output-file``      Path to the output file (in tsv format)
``--popmap-file``      Path to a popmap file indicating the sex of each individual
``--max-distance``     Maximum Levenstein distance between two sequences to group them in a locus (default 1)
``--threads``          Number of threads to use (default 1)
``--min-coverage``     Minimum coverage to consider a marker present in an individual (default: 1)
=====================  ===========

**Sample output**

::

    Locus    Marker      Sequence      Origin
        0        15      TGCA..TATT    Original
        0        27      TGCA..TAAT    Recovered
        1        43      TGCA..ATCG    Original
        1        86      TGCA..ATGG    Recovered


map
---

**Command**

::

    radsex map --input-file input_file_path --output-file output_file_path --popmap-file popmap_file_path --genome-file genome_file_path [ --min-coverage min_cov --min-quality min_quality --min-frequency min_frequency ]

The ``map`` command aligns all makers from a coverage table (obtained either from ``process``, ``subset``, or ``signif``) to a reference genome provided in fasta format. The output is a tabulated file where each line gives a marker ID, the contig where the marker mapped, the mapping position of the marker on this contig, the sex-bias of the marker (defined as M / Tm - F / Tf where M and F are the number of males and females in which the marker is present, and Tm and Tf are the total number of males and females in the population), the probability of association with sex for this marker (obtained with a chi-square test with Yate's correction for continuity), and the significativity of the association with sex after Bonferroni correction.

**Options**

=====================  ===========
Option                 Description
=====================  ===========
``--input-file``       Path to an coverage table obtained with ``process``, ``subset``, or ``signif``
``--output-file``      Path to the output file (in tsv format)
``--popmap-file``      Path to a popmap file indicating the sex of each individual
``--genome-file``      Path to a reference genome file in fasta format
``--min-coverage``     Minimum coverage to consider a marker present in an individual (default: 1)
``--min-quality``      Minimum mapping quality, as defined in BWA, to consider a sequence properly mapped (default: 20)
``--min-frequency``    Minimum frequency in at least one sex for a sequence to be retained (default: 0.25)
=====================  ===========

**Sample output**

::

    Marker        Contig      Position      SexBias        P     Signif
    0             LG09        10052920            0        1     False
    1             LG45         4008419            0        1     False
    2             LG06        20521435            0        1     False
    3             LG24         7643946         0.13     0.44     False
    4             LG06        16975491            0        1     False
    5             LG27        16580048            0        1     False
    6             LG49         7206356         0.03        1     False
    7             LG30         5571989            0        1     False
    8             LG05        20094761            0        1     False
    9             LG14        20088495            0        1     False
    10            LG34        11566459        -0.04        1     False
    11            LG21        17338149            0        1     False
    12            LG05        14652417         0.13     0.55     False
    13            LG25        23851527         0.75    0.001     True

freq
----

**Command**

::

    radsex freq --input-file input_file_path --output-file output_file_path [ --min-coverage min_cov ]

The ``freq`` command computes the distribution of markers in the population. The output is a tabulated file where the first column gives the number of individuals, and the second column gives the number of markers found in this number of individuals.

**Options**

=====================  ===========
Option                 Description
=====================  ===========
``--input-file``       Path to an coverage table obtained with ``process``
``--output-file``      Path to the output file (in tsv format)
``--min-coverage``     Minimum coverage to consider a marker present in an individual (default: 1)
=====================  ===========

**Sample output**

::

    Frequency    Count
            1       15
            2       37
            3       48
            4       43
            5      124

