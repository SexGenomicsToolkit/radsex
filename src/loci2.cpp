#include "loci.h"

void loci(Parameters& parameters) {

//    std::ofstream output_file;
//    output_file.open(parameters.output_file_path);

//    // Load popmap
//    std::unordered_map<std::string, bool> popmap = load_popmap(parameters);
//    uint n_indiv_total[2] {0, 0};
//    for (auto i: popmap) ++n_indiv_total[i.second];
//    uint min_indiv[2] {uint(n_indiv_total[0] * parameters.loci_min_individual_frequency), uint(n_indiv_total[1] * parameters.loci_min_individual_frequency)};

//    // Load individual depths from depth file
//    std::vector<std::string> header;
//    std::unordered_map<std::string, float> individual_depths_table;
//    std::ifstream depths_file;
//    depths_file.open(parameters.depths_file_path);
//    std::string temp = "";
//    std::getline(depths_file, temp);  // Header line not used in this analysis
//    if (depths_file) {
//        while (std::getline(depths_file, temp)) {
//            header = split(temp, "\t");
//            if (header.size() == 4) individual_depths_table[header[0]] = std::stof(header[2]);
//        }
//    }

//    float sum = 0;
//    for (auto i: individual_depths_table) sum += i.second;
//    sum /= (n_indiv_total[0] + n_indiv_total[1]);
//    for (auto& i: individual_depths_table) i.second /= sum;

//    // Load subset of markers
//    std::ifstream markers_subset_file;
//    std::vector<std::string> fields;
//    markers_subset_file.open(parameters.subset_file_path);
//    std::vector<Locus> markers_subset;
//    bool heterogametic_sex = 0;  // 0 --> females, 1 --> males
//    uint sex_count[2] {0, 0};
//    float sex_bias = 0;
//    uint n_markers = 0;

//    std::getline(markers_subset_file, temp);  // Header line
//    header = split(temp, "\t");

//    while(std::getline(markers_subset_file, temp)) {
//        fields = split(temp, "\t");
//        Locus locus;
//        locus.id = fields[0];
//        locus.sequence = fields[1];
//        for (uint i=2; i< fields.size(); ++i) locus.coverage.push_back(fields[i]);
//        locus.coverage = std::vector<std::string>(fields.begin() + 2, fields.end());
//        markers_subset.push_back(locus);
//        for (uint i=0; i< locus.coverage.size(); ++i) {
//            sex_count[popmap[header[i+2]]] += (std::stoul(locus.coverage[i]) > parameters.min_depth);
//        }
//        sex_bias += float(sex_count[1]) / float(n_indiv_total[1]) - float(sex_count[0]) / float(n_indiv_total[0]); // Average sex bias : > 0 when male-biased and < 0 when female-biased
//        ++n_markers;
//        sex_count[0] = 0;
//        sex_count[1] = 0;
//    }

//    sex_bias /= n_markers;
//    heterogametic_sex = (sex_bias < 0);  // We don't handle sex-bias = 0 for now.

//    // Get alleles with sex-biased depths from the markers depth table
//    std::ifstream markers_table_file;
//    markers_table_file.open(parameters.markers_table_path);
//    std::vector<Locus> potential_markers;

//    float rel_depth = 0.0;

//    if (markers_table_file) {

//        // First line is a comment with number of markers in the table
//        std::getline(markers_table_file, temp);
//        header = split(temp, " : ");
//        if (header.size() == 2) uint n_markers = static_cast<uint>(std::stoi(header[1]));

//        // Second line is the header. The header is used when printing the output.
//        std::getline(markers_table_file, temp);
//        output_file << temp << std::endl;
//        header = split(temp,"\t");
//        std::vector<ulong> individual_depths(header.size());
//        for (uint i=0; i<header.size(); ++i) {  // Transform individual depths table in a vector with indexing corresponding to field index in the markers table
//            (individual_depths_table.find(header[i]) != individual_depths_table.end()) ? individual_depths[i] = individual_depths_table[header[i]] : individual_depths[i] = 0;
//        }

//        // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
//        std::unordered_map<uint, uint> sex_columns = get_column_sex(popmap, header);

//        // Define variables used to read the file
//        temp = "";
//        char buffer[65536];
//        uint k = 0, field_n = 0, seq_count = 0;
//        Locus locus;

//        float relative_depth[2] {0.0, 0.0};
//        uint n_individuals_marker[2] {0, 0};

//        do {

//            // Read a chunk of size given by the buffer
//            markers_table_file.read(buffer, sizeof(buffer));
//            k = static_cast<uint>(markers_table_file.gcount());

//            for (uint i=0; i<k; ++i) {

//                // Read the buffer character by character
//                switch(buffer[i]) {

//                    case '\t':  // New field

//                        switch(field_n) {

//                            case 0:
//                                locus.id = temp;
//                                break;

//                            case 1:
//                                locus.sequence =temp;
//                                break;

//                            default:
//                                locus.coverage.push_back(temp);
//                                break;
//                        }

//                        temp = "";
//                        ++field_n;
//                        break;

//                    case '\n':  // New line (also a new field)

//                        locus.coverage.push_back(temp);

//                        n_individuals_marker[0] = 0;
//                        n_individuals_marker[1] = 0;
//                        relative_depth[0] = 0;
//                        relative_depth[1] = 0;
//                        rel_depth = 0;

//                        for (uint i=0; i<locus.coverage.size(); ++i) {

//                            if (std::stoul(locus.coverage[i]) >= parameters.min_depth) {
//                                relative_depth[sex_columns[i+2]] += static_cast<float>(std::stoul(locus.coverage[i])) * static_cast<float>(individual_depths[i+2]);
//                                ++n_individuals_marker[sex_columns[i+2]];
//                             }
//                        }

//                        if (n_individuals_marker[0] > 0) relative_depth[0] /= n_individuals_marker[0];
//                        if (n_individuals_marker[1] > 0) relative_depth[1] /= n_individuals_marker[1];

//                        if (n_individuals_marker[0] > 25 and n_individuals_marker[1] > 25) {
//                            if (relative_depth[!heterogametic_sex] > 0) rel_depth = static_cast<float>(relative_depth[heterogametic_sex]) / static_cast<float>(relative_depth[!heterogametic_sex]);
//                            if (rel_depth >= static_cast<float>(0.4) and rel_depth <= static_cast<float>(0.6)) potential_markers.push_back(locus);
//                        }


////                        // Count number of males and females with this marker
////                        for (uint i=0; i< locus.coverage.size(); ++i) {

////                            rel_depth = static_cast<float>(std::stoul(locus.coverage[i])) / static_cast<float>(individual_depths[i+2]);

////                            n_individuals_marker[sex_columns[i+2]] += (std::stoul(locus.coverage[i]) >= parameters.min_depth);

////                            if (sex_columns[i+2] == heterogametic_sex) {
////                                if (rel_depth >= parameters.loci_freq_het - parameters.loci_range_het and rel_depth <= parameters.loci_freq_het + parameters.loci_range_het) {
////                                    ++relative_depth[sex_columns[i+2]];
////                                }
////                            } else {
////                                if (rel_depth >= parameters.loci_freq_hom - parameters.loci_range_hom and rel_depth <= parameters.loci_freq_hom + parameters.loci_range_hom) {
////                                    ++relative_depth[sex_columns[i+2]];
////                                }
////                            }
////                        }

////                        if (n_individuals_marker[0] >= min_indiv[0] and n_individuals_marker[1] >= min_indiv[1]) {  // Retain a marker if enough individuals from each sex

////                            if (relative_depth[0] >= min_indiv[0] and relative_depth[1] >= min_indiv[1]) {
////                                potential_markers.push_back(locus);
////                            }

////                        }

//                        // Reset variables
//                        locus.id = "";
//                        locus.sequence = "";
//                        locus.coverage.resize(0);
//                        temp = "";
//                        field_n = 0;
//                        ++seq_count;
//                        if (seq_count % 1000000 == 0) {
//                            std::cout << " - Processed " << seq_count / 1000000 << "M. markers" << std::endl;
//                        }
//                        break;

//                    default:
//                        temp += buffer[i];
//                        break;
//                }
//            }

//        } while (markers_table_file);
//    }

//    const char* seq;
//    EdlibAlignResult alignment_result_f, alignment_result_r;
//    std::unordered_map<std::string, uint> marker_counts;
//    std::vector<std::pair<Locus, Locus>> final_loci;
//    std::string rev_seq = "";

//    std::cout << "Markers : " << markers_subset.size() << "  |  Potential markers : " << potential_markers.size() << std::endl;

//    for (auto pmarker: potential_markers) {

//        seq = pmarker.sequence.c_str();

//        for (auto marker: markers_subset) {

//            rev_comp(marker.sequence, rev_seq);
//            alignment_result_f = edlibAlign(seq, static_cast<int>(pmarker.sequence.size()), marker.sequence.c_str(), static_cast<int>(marker.sequence.size()), edlibDefaultAlignConfig());
//            alignment_result_r = edlibAlign(seq, static_cast<int>(pmarker.sequence.size()), rev_seq.c_str(), static_cast<int>(marker.sequence.size()), edlibDefaultAlignConfig());

//            if (alignment_result_f.status == EDLIB_STATUS_OK and alignment_result_f.editDistance <= static_cast<int>(parameters.loci_max_distance)) {

//                final_loci.push_back(std::pair<Locus, Locus>(marker, pmarker));
//                ++marker_counts[marker.id];
//                ++marker_counts[pmarker.id];

//            } else if (alignment_result_r.status == EDLIB_STATUS_OK and alignment_result_r.editDistance <= static_cast<int>(parameters.loci_max_distance)) {

//                marker.sequence = rev_seq;
//                final_loci.push_back(std::pair<Locus, Locus>(marker, pmarker));
//                ++marker_counts[marker.id];
//                ++marker_counts[pmarker.id];

//            }

//            edlibFreeAlignResult(alignment_result_f);
//            edlibFreeAlignResult(alignment_result_r);
//        }
//    }

//    std::cout << "Markers : " << markers_subset.size() << "  |  Retained markers : " << final_loci.size() << std::endl;

////    std::vector<uint> to_remove;
////    for (uint i=0; i<final_loci.size(); ++i) {
////        if (marker_counts[final_loci[i].first.id] > 1 or marker_counts[final_loci[i].second.id] > 1) to_remove.push_back(i);
////    }
////    std::sort(to_remove.begin(), to_remove.end(), std::greater<uint>());
////    for (auto index: to_remove) final_loci.erase(final_loci.begin() + index);

////    std::cout << "Markers : " << markers_subset.size() << "  |  Retained markers without duplicates : " << final_loci.size() << std::endl;

//    for (auto marker: final_loci) {
//        output_file << marker.first.id << "\t" << marker.first.sequence;
//        for (auto c: marker.first.coverage) output_file << "\t" << c;
//        output_file << "\n";
//        output_file << marker.second.id << "\t" << marker.second.sequence;
//        for (auto c: marker.second.coverage) output_file << "\t" << c;
//        output_file << "\n";
//    }

//    output_loci(parameters.output_file_path, final_loci, header);

}
