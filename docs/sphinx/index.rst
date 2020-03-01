.. RADSex documentation master file, created by
   sphinx-quickstart on Thu Sep 13 15:17:16 2018.

RADSex documentation
====================

RADSex is a software package to analyze RAD-Sequencing data. It is primarily designed to look compare female and male populations looking for sex signal, but it can be used to compare any two populations.

The core idea of RADSex is to compare presence / absence of non-polymorphic markers between individuals in two populations. RADSex does not allow mismatches when grouping reads into markers. This means that each allele in a polyallelic locus is represented as a separate marker, whereas other RAD-Sequencing analysis softwares would usually group these alleles in a single polymorphic marker. Separating alleles from polymorphic markers enables RADSex to easily detect sex-specific alleles, using only minimum depth of a marker as a parameter.

The main input of RADSex is a dataset of demultiplexed RAD reads, *i.e.* one reads file per individual. From this dataset, RADSex generates a table containing the depth of each marker in each individual. This table is then used to infer information about the type of sex-determination system, identify sex-biased markers, or align the markers to a reference genome. Several functions are also implemented to assist with general analysis of the dataset, for instance computing the frequencies of markers in individuals from the dataset or the median marker depth in each individual.

Results from RADSex can be visualized with the `radsex-vis <https://github.com/INRA-LPGP/radsex-vis>`_ R package. This R package provides easy-to-use functions to generate visual representations of your data.

RADSex's API documentation generated with Doxygen is available `here <../../../doxygen/html/index.html>`_

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
