#include "mapping.h"

//KSEQ_INIT(gzFile, gzread)


void mapping(Parameters& parameters) {

//    bwaidx_t *index;
//    gzFile sequences_file;
//    kseq_t *ks;
//    mem_opt_t *opt;

//    std::string genome_file_path = "/home/rferon/work/data/genomes/gadus_morhua/gadus_morhua_genome.fasta";
//    std::string sequences_file_path = "/home/rferon/work/analyses/radsex/results/genome_mapping/gadus_morhua/data/extracted_haplotypes.fasta";

//    std::string output_file_path = "/home/rferon/work/analyses/radsex/results/sex_distribution/esox_lucius_euro/test.tsv";
//    std::ofstream output_file;
//    output_file.open(output_file_path);

//    std::string rejects_file_path = "/home/rferon/work/analyses/radsex/results/sex_distribution/esox_lucius_euro/test_rejects.tsv";
//    std::ofstream rejects_file;
//    rejects_file.open(rejects_file_path);

//    // load the BWA index
//    index = bwa_idx_load(genome_file_path.c_str(), BWA_IDX_ALL);

//    if (NULL == index) {
//        fprintf(stderr, "Index load failed.\n");
//        exit(EXIT_FAILURE);
//    }

//    sequences_file = strcmp(sequences_file_path.c_str(), "-")? gzopen(sequences_file_path.c_str(), "r") : gzdopen(fileno(stdin), "r");

//    if (NULL == sequences_file) {
//        fprintf(stderr, "Couldn't open %s : %s\n",
//                strcmp(sequences_file_path.c_str(), "-") ? sequences_file_path.c_str() : "stdin",
//                errno ? strerror(errno) : "Out of memory");
//        exit(EXIT_FAILURE);
//    }

//    ks = kseq_init(sequences_file); // initialize the FASTA/Q parser
//    opt = mem_opt_init(); // initialize the BWA-MEM parameters to the default values

//    mem_alnreg_v ar;
//    mem_aln_t a, best;
//    std::vector<mem_aln_t> other_alignments;
//    uint i;
//    double p, sex_bias;
//    std::string cigar;

//    output_file << "Sequence" << "\t" << "Contig" << "\t" << "Position" << "\t" << "SexBias" << "\t" << "P" << "\n";

//    rejects_file << "Sequence" << "\t" << "Contig" << "\t" << "Position" << "\t" << "SexBias" << "\t" << "P" << "\t" << "Nmaps" << "\t" << "Quality" <<  "\n";

//    while (kseq_read(ks) >= 0) { // Loop over sequences in input file

//        ar = mem_align1(opt, index->bwt, index->bns, index->pac, ks->seq.l, ks->seq.s); // Map the sequence

//        int best_alignment[3] {0, -1, 0}; // Index, score, count


//        for (i = 0; i < ar.n; ++i) { // Loop over alignments

////            if (ar.a[i].secondary >= 0) continue; // Skip secondary alignments

//            cigar = "";
//            a = mem_reg2aln(opt, index->bns, index->pac, ks->seq.l, ks->seq.s, &ar.a[i]); // Get forward-strand position and CIGAR

////            if (ar.a[i].score > best_alignment[1]) {
////                best_alignment[0] = i;
////                best_alignment[1] = ar.a[i].truesc;
////                best_alignment[2] = 0;
////            } else if (ar.a[i].score == best_alignment[1]) {
////                ++best_alignment[2];
////            }

//            for (int k = 0; k < a.n_cigar; ++k) {
//                cigar += std::to_string(a.cigar[k]>>4);
//                cigar += "MIDSH"[a.cigar[k]&0xf];
//            }

//            if (ks->name.s == std::string("40329_34_32")) {
//                std::cout << ks->name.s << "\t" << index->bns->anns[a.rid].name << "\t" << a.pos << "\t" << a.mapq << "\t" << a.score << "\t" << cigar << "\n";
//            }

//            if (a.score > best_alignment[1]) {
//                best_alignment[0] = i;
//                best_alignment[1] = a.score;
//                best_alignment[2] = 0;
//                best = a;
//                other_alignments.resize(0);
//            } else if (a.score == best_alignment[1]) {
//                ++best_alignment[2];
//                other_alignments.push_back(a);
//            }

//            free(a.cigar); // Deallocate CIGAR
//        }

//        p = 0.75;
//        sex_bias = 0.5;

//        if (best_alignment[2] < 1 and best.mapq > 19) {
//            output_file << ks->name.s << "\t" << index->bns->anns[best.rid].name << "\t" << best.pos << "\t" << sex_bias << "\t" << p << "\n";
//        } else {
//            rejects_file << ks->name.s << "\t" << index->bns->anns[best.rid].name << "\t" << best.pos << "\t" << sex_bias << "\t" << p << "\t" <<
//                            best_alignment[2] << "\t" << best.mapq << "\t" << best.score << "\t";
//            for (auto al: other_alignments) {
//                rejects_file << index->bns->anns[al.rid].name << "::" << al.pos << "::" << al.mapq << "::" << al.score << ",";
//            }
//            rejects_file << "\n";
//        }

////        if (a.mapq > 19 and best_alignment[2] < 1) { // Keep reads with mapping quality > 20 and no other mapping positions
////            output_file << ks->name.s << "\t" << index->bns->anns[a.rid].name << "\t" << a.pos << "\t" << sex_bias << "\t" << p << "\n";
////        } else {
////            rejects_file << ks->name.s << "\t" << index->bns->anns[a.rid].name << "\t" << a.pos << "\t" << sex_bias << "\t" << p << "\n";
////        }

////        if (best_alignment[2] > 1) std::cout << ks->name.s << "\t" << index->bns->anns[a.rid].name << "\t" << a.pos << "\t" << sex_bias << "\t" << p << std::endl;

//        free(ar.a); // Deallocate the hit list
//    }

//    free(opt);
//    kseq_destroy(ks);
//    gzclose(sequences_file);
//    bwa_idx_destroy(index);
}
