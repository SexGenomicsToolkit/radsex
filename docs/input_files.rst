Input file formats
==================

Reads files
-----------

RADSex accepts demultiplexed reads files as first input. RADSex should work with any demultiplexed RAD-sequencing reads files regardless of technology (single / double digest) or enzyme. Input files can be in fasta or fastq formats, and can be compressed. RADSex uses file extensions to detect input files, and supports the following extensions: **.fa**, **.fa.gz**, **.fq**, **.fq.gz**, **.fasta**, **.fasta.gz**, **.fastq**, **.fastq.gz**, **.fna**, and **.fna.gz**.

.. _population-map:

Population map
--------------

A population map file is a tabulated file (TSV, tab as a separator) without header, with individual ID in the first column and sex in the second column. Sex is encoded as **"M"** for males, **"F"** for females, and **"N"** for undetermined. An example of population map is given below:

::

	individual_1    M
	individual_2    M
	individual_3    F
	individual_4    N
	individual_5    F

Individual IDs can be anything, but it is important that they correspond to the name of the demultiplexed files.
For instance, the reads file for *individual_1* should be named `individual_1.fastq.gz` (or any fasta/fastq format supported by your demultiplexer).

If you are using Stacks with a barcodes file for demultiplexing, just make sure that individual IDs in the barcodes file and in the population map are the same.


Chromosomes names file
----------------------

Genome-wide results from the ``map`` command are visualized using the ``plot_genome()`` function of ``radsex-vis``.
This function can automatically detect chromosomes in the reference file if their name starts with 'LG' or 'chr' (case unsensitive). If this is not the case, you should provide a chromosomes names file to ``plot_genome()``.
This file should be a tabulated file without header, with scaffold ID in the reference in the first column and corresponding chromosome name in the second column.

An example of chromosomes names file is given below for the `Northern Pike <https://www.ncbi.nlm.nih.gov/genome/?term=esox%20lucius>`_ genome.

::

	NC_025968.3     LG01
	NC_025969.3     LG02
	NC_025970.3     LG03
	NC_025971.3     LG04
	NC_025972.3     LG05
	NC_025973.3     LG06
	NC_025974.3     LG07
	NC_025975.3     LG08
	NC_025976.3     LG09
	NC_025977.3     LG10
	NC_025978.3     LG11
	NC_025979.3     LG12
	NC_025980.3     LG13
	NC_025981.3     LG14
	NC_025982.3     LG15
	NC_025983.3     LG16
	NC_025984.3     LG17
	NC_025985.3     LG18
	NC_025986.3     LG19
	NC_025987.3     LG20
	NC_025988.3     LG21
	NC_025989.3     LG22
	NC_025990.3     LG23
	NC_025991.3     LG24
	NC_025992.3     LG25

.. note:: Any scaffold included in the chromosomes names file will be considered a chromosome to be plotted as a sector. In most NCBI genomes, mitochondria are also named NC_XXX. As mitochondria are usually too small to be included as a sector in the circos plot, you should not add them to the chromosomes names file.
