/*
* Copyright (C) 2020 Romain Feron
* This file is part of RADSex.

* RADSex is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* RADSex is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with RADSex.  If not, see <https://www.gnu.org/licenses/>.
*/

/*!
 * @file map.h
 * @brief Defines the Map class implementing the "map" analysis.
*/

#pragma once
#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "bwa/bwamem.h"
#include "bwa/kseq.h"
#include "analysis.h"
#include "markers_table.h"
#include "parameters.h"
#include "popmap.h"
#include "stats.h"
#include "utils.h"


/*!
 * \brief AlignedMarker struct
 *
 * Store information about a marker aligned to the assembly.
 */

struct AlignedMarker {

    std::string id;   ///< Marker id from the markers depth table (e.g. "1202565")
    std::string contig;   ///< Contig to which the marker was aligned in the best alignment
    int64_t position;   ///< Position on the contig in the best alignment
    float bias;   ///< Group bias for the marker [-1, 1]
    double p;   ///< Probability of association with group for the marker

};


/*!
 * \brief MapResults struct
 *
 * Store results for the "map" analysis.
 */

struct MapResults {

    std::vector<AlignedMarker> aligned_markers;   ///< Vector storing all markers aligned to the assembly
    uint64_t n_markers = 0;  ///< Number of markers present in at least one individual with depth higher than min_depth (for Bonferroni correction)

};


/*!
 * \brief Implements the "map" analysis
 *
 * Align all markers from a markers depth table to an assembly using the "mem" algorithm implemented in the BWA library (https://github.com/lh3/bwa). \n
 * For all aligned markers, compute probability of association with groups and group bias (bias score between -1 and 1). \n
 * After all markers are processed, generate a tabulated output file with columns: \n
 * Contig | Position | Length | Marker_id | Bias | P | P corrected | Signif
 *
 */

class Map: public Analysis {

    public:

        /*!
         * \brief Default Map constructor
         */

        Map() {};


        /*!
         * \brief Standard Map constructor
         *
         * \param parameters      Parameters object storing the value of all RADSex parameters
         * \param compare_groups  If true, groups are checked when loading the popmap
         * \param store_groups    If true, group info is computed for each marker when parsing the markers table
         * \param store_sequence  If true, marker sequences are stored for each marker when parsing the markers table
         */

        Map(Parameters& parameters, bool compare_groups = true, bool store_groups = true, bool store_sequence = true) : Analysis(parameters, compare_groups, store_groups, store_sequence) {};


        /*!
         * \brief Extra setup steps
         *
         * Load BWA index (build it if it's missing), initalize BWA mem parameters, load contig lengths from the genome file, and initialize output file.
         */

        void extra_setup();


        /*!
         * \brief Process a single marker
         *
         * Align a marker to the genome and compute probability of association with group and group bias if the marker was aligned.
         *
         * \param marker Current marker info stored in a Marker instance.
         */

        void process_marker(Marker& marker);


        /*!
         * \brief Generate the output file
         *
         * Generate a tabulated output file with columns: \n
         * Contig | Position | Length | Marker_id | Bias | P | Signif
         */

        void generate_output();

    private:

        MapResults results;  ///< DistribResults instance to store markers aligned to the genome and total number of retained markers
        std::unordered_map<std::string, uint64_t> contig_lengths;   ///< Map storing the genome's contig lengths {contig name -> contig length}

        uint16_t min_individuals = 0;   ///< Minimum number of individuals in which a marker is present to retain this marker (based on --min-freq)
        bwaidx_t* index;   ///< Pointer to a bwa index created with bwa_idx_build() and loaded with bwa_idx_load()
        mem_opt_t* bwa_mem_options;   ///< Pointer to BWA mem options initialized with mem_opt_init()
        mem_alnreg_v alignment_results;   ///< Store BWA mem alignment results from mem_align1()
        mem_aln_t best_alignment;   ///< Store a single BWA mem alignment results info obtained from a mem_alnreg_v object with mem_reg2aln()
        double chi_squared;   ///< Store result of chi-squared test on number of individuals from each group
        int best_alignments[3] {0, -1, 0};  ///< Store information about best alignments in a mem_alnreg_v object: {index, score, count}
        AlignedMarker aligned_marker;   ///< Store information about an aligned marker

        std::ofstream output_file;   ///< "map" output file


        /*!
         * \brief Load contig lengths
         *
         * Compute contig lengths from a genome file.
         */

        void load_contig_lengths();


        /*!
         * \brief Load a BWA index
         *
         * Check if BWA index files exist and build the index if they are missing, then load the index with bwa_index_load().
         */

        void load_bwa_index();
};
