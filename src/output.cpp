#include "output.h"

void output_process(std::string& output_file_path, std::vector<std::string>& individuals, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, uint min_depth) {

    /* Input:
     * - Path to an output file
     * - A list of individual names
     * - A matrix of results [Individual: [Sequence, Coverage]]
     * Output:
     * - A matrix of depth [Individual: [Sequence, Coverage]]
     */

    FILE* output_file;
    output_file = fopen(output_file_path.c_str(), "w");

    uint id = 0;

    // Comment lines
    std::fprintf(output_file, "#Number of markers : %i\n", static_cast<int>(results.size()));

    // Header
    std::fprintf(output_file, "id\tsequence");
    for (auto i: individuals) {
        std::fprintf(output_file, "\t%s", i.c_str());
    }
    std::fprintf(output_file, "\n");

    bool print = false;
    // Fill line by line
    for (auto r: results) {
        for (auto i: r.second) {
            if (i.second >= min_depth) {
                print = true;
                break;
            }
        }
        if (print) {
            std::fprintf(output_file, "%i\t%s", id, r.first.c_str());
            for (auto i: individuals) {
                std::fprintf(output_file, "\t%i", r.second[i]);
            }
            std::fprintf(output_file, "\n");

            ++id;
        }
        print = false;
    }

    fclose(output_file);
}



void output_distrib_matrix(std::string& output_file_path, sd_table& results, uint n_males, uint n_females) {

    /* Input:
     * - Path to an output file
     * - A matrix of counts [Males: [Females: Count]]
     * Output:
     * - A matrix of counts (males in columns and females in rows)
     */

    std::ofstream output_file;
    output_file.open(output_file_path);

    results[0][0].first = 0; // Sequences found in none of the individuals (after filtering for minimum coverage) should not appear in the results

    // Generate output
    for (uint f=0; f <= n_females; ++f) {
        for (uint m=0; m <= n_males; ++m) {
            output_file << results[m][f].first;
            (m < n_males) ? output_file << "\t" : output_file << "\n";
        }
    }
}



void output_distrib(std::string& output_file_path, sd_table& results, uint n_males, uint n_females, float signif_threshold, bool disable_correction) {

    /* Input:
     * - Path to an output file
     * - A matrix of counts [Males: [Females: Count]]
     * Output: a table with the following structure:
     * Number of males | Number of females | Number of markers   | P-value    | Significance
     *     <int>       |       <int>       |       <int>         | <float>    |    <bool>
     */

    std::ofstream output_file;
    output_file.open(output_file_path);

    // Output file header
    output_file << "Males" << "\t" << "Females" << "\t" << "Markers" << "\t" << "P" << "\t" << "Signif" << "\n";

    uint n_markers = 0;

    // Determine the total number of markers
    for (uint f=0; f <= n_females; ++f) {
        for (uint m=0; m <= n_males; ++m) {
            if (f + m != 0) n_markers += results[m][f].first; // Exclude markers present in 0 individuals
        }
    }

    if (not disable_correction) signif_threshold /= n_markers;

    // Generate output file
    for (uint m=0; m <= n_males; ++m) {
        for (uint f=0; f <= n_females; ++f) {
            if (f + m != 0) {
                output_file << m << "\t" << f << "\t" << results[m][f].first << "\t" << results[m][f].second << "\t" <<
                               (static_cast<float>(results[m][f].second) < signif_threshold ? "True" : "False") << "\n";
            }
        }
    }
}



void output_loci(std::string& output_file_path, std::vector<std::pair<Locus, Locus>> loci, std::vector<std::string>& header) {

    /* TODO
     */

    std::ofstream output_file;
    output_file.open(output_file_path);

    output_file << "Locus_ID" << "\t" << "Het_marker_ID" << "\t" << "Hom_marker_ID" << "\t" << "Het_marker_sequence" << "\t" << "Hom_marker_sequence" ;
    for (uint i=2; i<header.size(); ++i) output_file << "\t" << header[i];
    output_file << "\n";

    for (auto locus: loci) {
        output_file << locus.first.id + "_" + locus.second.id << "\t" << locus.first.id << "\t" << locus.second.id << "\t" << locus.first.sequence << "\t" << locus.second.sequence;
        for (uint i=0; i<locus.first.coverage.size(); ++i) {
            output_file << "\t" << locus.first.coverage[i] << "/" << locus.second.coverage[i];
        }
        output_file << "\n";
    }
}



// Create output file for the mapping analysis
void output_map(std::string& output_file_path, std::vector<MappedSequence> sequences, float signif_threshold, bool disable_correction) {

    std::ofstream output_file;
    output_file.open(output_file_path);

    output_file << "Sequence" << "\t" << "Contig" << "\t" << "Position" << "\t" << "SexBias" << "\t" << "P" << "\t" << "Signif" << "\n";

    if (not disable_correction) signif_threshold /= sequences.size();

    for (auto s: sequences) {
        output_file << s.id << "\t" << s.contig << "\t" << s.position << "\t" << s.sex_bias << "\t" << s.p << "\t" <<
                       (static_cast<float>(s.p) < signif_threshold ? "True" : "False") << "\n";
    }

    output_file.close();
}
