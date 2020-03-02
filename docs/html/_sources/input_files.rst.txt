Input files
===========

.. _reads-file:

Reads files
-----------

RADSex accepts demultiplexed reads files as input for ``process``. RADSex should work with any demultiplexed RAD-sequencing reads files regardless of technology (single / double digest) or enzyme. RADSex cannot support paired-end reads because insert sizes are variable, and thus the second read in a pair does not cover a consistent region of the genome. 

Input files can be in fasta or fastq formats and can be compressed with gzip. RADSex uses file extensions to detect input files and supports the following extensions: **.fa**, **.fa.gz**, **.fq**, **.fq.gz**, **.fasta**, **.fasta.gz**, **.fastq**, **.fastq.gz**, **.fna**, and **.fna.gz**.

Individual IDs are inferred from file names, *e.g.* RADSex will attribute the ID **individual_1** to the reads file **individual1.fastq.gz**.


.. _population-map:

Population map
--------------

A population map file is a headerless TSV file (*i.e.* a tabulated file using "\\t" - the "tab" character - as a separator) with individual ID in the first column and group in the second column. Groups can be any value and there can be more than two groups. However, most radsex analyses perform pairwise comparisons between groups and will require specifying the groups to compare with the ``--groups`` parameters (*e.g.* ``--groups males,females``). If the popmap contains two groups, these groups will be used for pairwise group comparisons in the order they are found in the popmap (this order can be overridden with ``--groups``).

An example of population map is given below:

::

	individual_1    M
	individual_2    M
	individual_3    F
	individual_4    N
	individual_5    F

Individual IDs can be any value, but it is important that they correspond to the name of the demultiplexed files (without the extension). For instance, the reads file for *individual_1* should be named *individual_1.fastq.gz* (or any fasta/fastq format supported by your demultiplexer).

If you are using Stacks with a barcodes file to demultiplexing the RAD-sequencing reads, just make sure that individual IDs in the barcodes file and in the population map are the same.


.. _chromosomes-names:

Chromosomes names file
----------------------

Genome-wide results from the ``map`` command are visualized using the ``plot_genome()`` function of ``radsex-vis``. This function automatically detects chromosomes if their name starts with "LG", "CHR", or "NC" (case unsensitive). If this is not the case, or if you would like to rename the chromosomes (for instance to shorten NCBI "NC_XXX" IDs), you should provide a chromosomes names file to ``plot_genome()``.

The chromosomes names file is a headerless tabulated file with chromosome ID (from the genome file, *e.g.* "NC_XXXX") in the first column and corresponding chromosome name in the second column (*e.g.* "Chrom1").

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
