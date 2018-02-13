#include "output.h"
#include <iostream>


void output_process_reads(std::string& output_file_path, std::vector<std::string>& individuals, std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>>& results, uint min_cov) {

    /* Input:
     * - Path to an output file
     * - A list of individual names
     * - A matrix of results [Individual: [Sequence, Coverage]]
     * Output:
     * - A matrix of coverage [Individual: [Sequence, Coverage]]
     */

    FILE* output_file;
    output_file = fopen(output_file_path.c_str(), "w");

    uint id = 0;

    // Prints the header
    std::fprintf(output_file, "id\tsequence");
    for (auto i: individuals) {
        std::fprintf(output_file, "\t%s", i.c_str());
    }
    std::fprintf(output_file, "\n");

    bool print = false;
    // Fill line by line
    for (auto r: results) {
        for (auto i: r.second) {
            if (i.second > min_cov) {
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



void output_sex_distribution(std::string& output_file_path, std::unordered_map<uint, std::unordered_map<uint, uint64_t>>& results, uint n_males, uint n_females) {

    /* Input:
     * - Path to an output file
     * - A matrix of counts [Males: [Females: Count]]
     * Output:
     * - A matrix of counts (males in columns and females in rows)
     */

    std::ofstream output_file;
    output_file.open(output_file_path);

    uint i = 0;

    results[0][0] = 0; // Sequences found in none of the individuals (after filtering for minimum coverage) should not appear in the results

    for (uint f=0; f < n_females; ++f) {
        for (uint m=0; m < n_males; ++m) {
            output_file << results[m][f];
            if (i < n_males - 1) output_file << "\t";
            ++i;
        }
        output_file << "\n";
        i=0;
    }
}



void output_group_loci(std::string& output_file_path, std::unordered_map<std::string, std::vector<Locus>>& results) {

    /* Input:
     * - Path to an output file
     * - A matrix of loci [Sequence ID: [Associated sequences]]
     * Output:
     * - A table with following columns:
     * Locus ID | Sequence ID | Sequence Status | Sequence | Cov Ind. 1 | Cov Ind. 2 ...
     */

    std::ofstream output_file;
    output_file.open(output_file_path);

    std::string seq_id;
    uint locus_id = 0;

    for (auto sequence: results) {
        seq_id = sequence.first;

        // First iteration to output the original sequence
        for (auto locus: sequence.second) {
            if (locus.id == seq_id) {
                output_file << locus_id << "\t" << locus.id << "\t" << locus.sequence << "\t";
                if (seq_id == locus.id) output_file << "Original" << "\t"; else output_file << "Recovered" << "\t";
                for (uint i=0; i<locus.coverage.size(); ++i) {
                    output_file << locus.coverage[i];
                    if (i < locus.coverage.size() - 1) output_file << "\t";
                }
                output_file << "\n";
            }
        }

        // Second iteration to output the other sequences (this is not optimized but this step is not intensive anyway)
        for (auto locus: sequence.second) {
            if (locus.id != seq_id) {
                output_file << locus_id << "\t" << locus.id << "\t" << locus.sequence << "\t";
                if (seq_id == locus.id) output_file << "Original" << "\t"; else output_file << "Recovered" << "\t";
                for (uint i=0; i<locus.coverage.size(); ++i) {
                    output_file << locus.coverage[i];
                    if (i < locus.coverage.size() - 1) output_file << "\t";
                }
                output_file << "\n";
            }
        }
        ++locus_id;
    }
}
