.. RADSex documentation master file, created by
   sphinx-quickstart on Thu Sep 13 15:17:16 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

RADSex documentation
====================

RADSex is a software package to analyze RAD-Sequencing data. It is primarily designed to look for sex signal, but it can be used to compare any two populations.

The core idea of RADSex is to compare presence / absence of non-polymorphic markers between individuals in two populations. RADSex does not allow mismatches when grouping reads into markers. This means that each allele in a polyallelic gene is represented as a separate marker, whereas other RAD-Sequencing analysis softwares would usually group these alleles in a single polymorphic marker. Separating alleles from polymorphic markers enables RADSex to easily detect sex-specific alleles, using only minimum coverage of a marker as a parameter.

The basic input file of RADSex is a dataset of demultiplexed RAD reads. From this dataset, RADSex generates a table of coverage for each sequence in each individual. This table is then used to infer information about the type of sex-determination system, identify sex-biased sequences, map the RAD sequences to a reference genome, and recover potential polymorphic sex-biased markers.

Results from RADSex can be visualized with the `radsex-vis` R package, available here: https://github.com/INRA-LPGP/radsex-vis. The `radsex-vis` R package provides easy-to-use functions to generate visual representations of your data.

Documentation summary
---------------------

.. toctree::
   :maxdepth: 2

   getting_started
   example
   usage
   input_files
   output_files
   license
