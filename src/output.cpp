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



void output_distrib_matrix(std::string& output_file_path, sd_table& results, uint n_group1, uint n_group2) {

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
    for (uint f=0; f <= n_group1; ++f) {
        for (uint m=0; m <= n_group2; ++m) {
            output_file << results[f][m].first;
            (m < n_group1) ? output_file << "\t" : output_file << "\n";
        }
    }
}



void output_distrib(std::string& output_file_path, sd_table& results, uint n_group1, uint n_group2, std::string& group1, std::string& group2,
                    float signif_threshold, bool disable_correction) {

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
    output_file << group1 << "\t" << group2 << "\t" << "Markers" << "\t" << "P" << "\t" << "Signif" << "\n";

    uint n_markers = 0;

    // Determine the total number of markers
    for (uint f=0; f <= n_group1; ++f) {
        for (uint m=0; m <= n_group2; ++m) {
            if (f + m != 0) n_markers += results[f][m].first; // Exclude markers present in 0 individuals
        }
    }

    if (not disable_correction) signif_threshold /= n_markers;

    // Generate output file
    for (uint m=0; m <= n_group1; ++m) {
        for (uint f=0; f <= n_group2; ++f) {
            if (f + m != 0) {
                output_file << m << "\t" << f << "\t" << results[m][f].first << "\t" << results[m][f].second << "\t" <<
                               (static_cast<float>(results[m][f].second) < signif_threshold ? "True" : "False") << "\n";
            }
        }
    }
}

