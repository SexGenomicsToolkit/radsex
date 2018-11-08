Detailed example
================

In this example, we will run RADSex on a public *Oryzias latipes* RAD-Sequencing dataset. We will detail each step of the process, highlight important details, and show how to use the R package ``radsex-vis`` to generate plots from the output of ``radsex``. This guide assumes that ``radsex`` and the ``radsex-vis`` package have already been installed. For specific instruction about installing ``radsex`` and ``radsex-vis``, check the :ref:`install-release` section. All reported times and resources usage were measured on a desktop computer with an Intel i7-8700K 4.7 GHz processor, 32 Gb of memory, and a standard 7200 RPM Hard Disk Drive.

Preparing the data
------------------

The RAD-Sequencing datasets used in this example are available on the Sequence Read Archive on NCBI. The reads were demultiplexed before being deposited on NCBI, and samples were grouped in two projects, males and females. The accession number for **female** samples is **SRS662264**, and the accession number for **male** samples is **SRS662265**. For convenience, simple scripts to download male and female samples from the EBI ftp can be found `here <https://github.com/RomainFeron/RadSex/tree/master/example/oryzias_latipes/data/download_female_samples.sh>`__ for female samples and `here <https://github.com/RomainFeron/RadSex/tree/master/example/oryzias_latipes/data/download_male_samples.sh>`_ for male samples. This dataset was published in `Wilson et al 2014 <http://www.genetics.org/content/early/2014/09/18/genetics.114.169284>`__.

The population map describing the sex of each samples for this dataset can be found `here <https://github.com/RomainFeron/RadSex/tree/master/example/oryzias_latipes/data/population_map.tsv>`__. The genome used for mapping markers with `radsex map` was that of a HSOK strain, NCBI accession number **GCA_002234695.1** (`link <https://www.ncbi.nlm.nih.gov/assembly/GCA_002234695.1>`_). The chromosomes names file used to display chromosomes with nicer names in the genome mapping plot can be found `here <https://github.com/RomainFeron/RadSex/tree/master/example/oryzias_latipes/data/chromosomes_names.tsv>`__.

.. note:: RADSex uses file names to generate individual IDs. Therefore, individual names in the population map should correspond to the file names without their extensions. Check the file names and population map provided above for an example of how to build the population map from file names. More details about the population map can be found in the :ref:`population-map` section.

From now on, we will assume the following directory structure:

::

    .
    ├─── samples
    |     ├────── xxx.fastq.gz
    |     ├────── xxx.fastq.gz
    |     ├────── ...
    |     └────── xxx.fastq.gz
    └─── popmap.tsv


Generating a coverage table for the entire dataset
--------------------------------------------------

The first step of RADSex is to create a table containing the coverage of each marker in each individual for the entire dataset; a marker is defined as a non-polymorphic sequence (no mismatches or SNPs). This step is performed with the ``process`` command :

::

    radsex process --input-dir samples --output-file coverage_table.tsv --threads 8 --min-cov 1

The ``--input-dir`` parameter specifies the location of the demultiplexed reads directory, which is *samples* in our case. The ``--output-file`` parameter specifies the location of the output file (*i.e.* the table of coverage), which is *coverage_table.tsv* here. This step can be parallelized using the ``--threads`` parameters, which we set to *8* in our example; you should adjust this value based on your computer's specifications. Finally, the ``--min-cov`` parameter specifies the minimum coverage for a marker to be considered present in an individual; if a marker has a coverage lower than the value of ``--min-cov`` in every individual, it will not be retained in the coverage table.

The resulting file *coverage_table.tsv* will be used as a base for all analyses implemented in ``radsex``, but it is not used for any ``radsex-vis`` plots. For more information about this file, check the :ref:`coverage-table-file` section.

.. note:: In most cases, we advise to keep the value of ``--min-cov`` to 1 in order to retain all the information from the dataset in this step. Filtering for minimum coverage should be done in the following analysis steps, and it will be easier to try several minimum coverage values this way. If you are certain that all individuals in your dataset have high coverage, and you do not plan to run analyses with a minimum coverage of 1, you can increase this threshold.

With our setup, using 8 cores, this step completed in **9 min 25 seconds** with a peak memory usage of **10.3 GB**. The resulting coverage table used 5.1 GB of disk space.


Computing the distribution of markers between sexes
---------------------------------------------------

The main analysis implemented in ``radsex`` computes a table summarizing the distribution of all markers between males and females. This analysis is performed with the ``distrib`` command:

::

    radsex distrib --input-file coverage_table.tsv --output-file distribution.tsv --popmap-file popmap.tsv --min-cov 5``

The ``--input-file`` parameter specifies the location of the coverage table generated in the previous step, which was *coverage_table.tsv* in our case. The ``--output-file`` parameter specifies the location of the output file, *i.e* the table of distribution of markers between males and females, which is *distribution.tsv* here. The ``--popmap`` parameter specifies the location of the population map (see the xx section for details), which we named *popmap.tsv* in this example. Finally, the ``--min-cov`` parameter specifies the minimum coverage to consider a marker present in an individual, and was set to *5* here.

With our setup, this step completed in **36 seconds** with a peak memory usage of **4 Mb**.

The resulting file *distribution.tsv* is a tabulated file described in the xxx section. This file can be visualized with ``radsex-vis`` using the ``plot_sex_distribution`` function:

::

    radsex-vis::plot_sex_distribution(input_file_path = "distribution.tsv", output_file_path = "distribution.png")

To generate a basic plot, the only required parameter is ``input_file_path``, which should contain the full path to the distribution table (simplified in this example). The ``output_file_path`` parameters specifies the path to an output file where the figure will be saved; if this parameter is not specified, the plot will be generated in the default R graphic device. For a full description of the ``plot_sex_distribution`` function, including additional parameters, check the TODO_SECTION.

The resulting figure is displayed below:

.. image:: ../example/figures/distribution.png

This figure is a tile plot with number of males on the x-axis and number of females on the y-axis. The color of a tile at coordinates (**x**, **y**) indicates the number of markers that were present in any **x** males and any **y** females. For instance, in this figure, there were between 25 and 99 markers found in 29 males (not necessarily always the same 29 males) and in 0 females. Tiles for which association with sex is significant (chi-squared test, using Bonferroni correction) are highlighted in red. Many markers found predominantly in males are significantly associated with sex, indicating that an XX/XY system determines sex in this species. Interestingly, there are no markers found in all males and absent from all females, *i.e* no markers found at positions



Extracting markers significantly associated with sex
----------------------------------------------------

Markers significantly associated with sex can be obtained with the ``signif`` command:

::

    radsex signif --input-file coverage_table.tsv --output-file markers.tsv --popmap-file popmap.tsv --min-cov 5 [ --output-format fasta ]

In this example, the input file ``--input-file`` is the coverage table generated in the :ref:`computing-cov-table` step, and the markers significantly associated with sex are outputed in **markers.tsv**. The sex of each individual in the population is given by **popmap.tsv** (see the [popmap section](#population-map) for details), and the minimum coverage to consider a sequence present in an individual is set to 5, meaning that markers with coverage lower than 5 in an individual will not be considered present in this individual.

By default, the ``signif`` function generates an output file in the same format as the coverage table. However, the sequences can be exported to fasta using the ``--output-format`` parameter (see TODO SECTION).

The coverage table generated by ``signif`` can be visualized with the ``plot_coverage()`` function of `RADSex-vis <https://github.com/RomainFeron/RADSex-vis>`_, which generates a heatmap showing the coverage of each sequence in each individual.


Mapping markers to a reference genome
-------------------------------------

The markers can be mapped to a reference genome using the ``map`` command:

::

    radsex map --input-file coverage_table.tsv --output-file mapping.tsv --popmap-file popmap.tsv --genome-file genome.fasta --min-quality 20 --min-frequency 0.1 --min-cov 5

In this example, the input file ``--input-file`` is the coverage table generated in the :ref:`computing-cov-table` step, the mapping results will be stored in **sequences.tsv**, and the path to the reference genome file is given by ``--genome-file``. The sex of each individual in the population is given by **popmap.tsv** (see the [popmap section](#population-map) for details), and the minimum coverage to consider a marker present in an individual is set to 5, meaning that markers with coverage lower than 5 in an individual will not be considered present in this individual. The parameter ``--min-quality`` specifies the minimum mapping quality (as defined in `BWA <http://bio-bwa.sourceforge.net/bwa.shtml>`_) to consider a marker properly mapped, and is set to 20 in this example. The parameter ``--min-frequency`` specifies the minimum frequency of a marker in at least one sex; it is set to 0.1 here, meaning that only sequences present in at least 10% of individuals of one sex are retained for mapping.

The resulting file ``mapping.tsv`` is a table with five columns:

* **Sequence :** ID of the mapped sequence.
* **Contig :** ID of the contig where the sequence mapped.
* **Position :** position of the mapped sequence on the contig.
* **SexBias :** sex-bias of the mapped sequence, defined as (Males / Total males ) - (Females / Total females), where *Males* and *Females* are the number of males and number of females in which the sequence is present, respectively, and *Total males* and *Total females* are the total number of males and females in the population, respectively.
* **P :** p-value of a chi-squared test for association with sex.
* **Signif** : significant association with sex (True / False).

The mapping results generated by ``map`` can be visualized with the ``plot_genome()`` function of `RADSex-vis <https://github.com/RomainFeron/RADSex-vis>`_, which generates a circular plot with the sex-bias and association with sex of each marker mapped on the genome.
Mapping results for a specific contig can be visualized with the ``plot_scaffold()`` function to show the same metrics for a single contig.

