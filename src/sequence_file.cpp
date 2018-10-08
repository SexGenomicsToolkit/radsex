#include "sequence_file.h"

// List of currently supported file types with extensions
std::vector<std::string> extensions {".fq.gz", ".fq", ".fastq.gz", ".fastq", ".fasta.gz", ".fasta", ".fa.gz", ".fa", ".fna.gz", ".fna"};

std::vector<InputFile> get_input_files(const std::string& input_dir_path) {

    /* Gets all relevant files (from supported formats) from the input directory.
     * Files are stored as InputFile object which contains all relevant information about the file.
     * The function returns a vector of InputFiles.
     */

    DIR* dir = opendir(input_dir_path.c_str());

    if(!dir) {
        std::cout << std::endl << "** Error: could not read the directory \"" << input_dir_path << "\"" << std::endl;
        exit(0);
    }

    struct dirent* dir_content;
    std::vector<InputFile> files;
    std::string current_file, extension, individual_name;
    std::vector<std::string> split_name;
    InputFile temp;

    while ((dir_content = readdir(dir))) {

        current_file = dir_content->d_name;
        split_name = split(current_file, ".");
        size_t s = split_name.size();
        extension = "";
        individual_name = split_name[0];

        // Get file name and extension (even when there is "." in the file name)
        if (s > 1) {
            if (split_name[s - 1] == "gz" and s > 2) {
                extension = "." + split_name[s - 2] + "." + split_name[s - 1];
                for (uint i=1; i<split_name.size() - 2; ++i) individual_name += "." + split_name[i];
            } else {
                extension = "." + split_name[s - 1];
                for (uint i=1; i<split_name.size() - 1; ++i) individual_name += "." + split_name[i];
            }
        }

        if(std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
            temp.individual_name = individual_name;
            temp.path = input_dir_path + current_file;
            temp.extension = extension;
            temp.processed = false;
            files.push_back(temp);
        }
    }

    if (files.size() == 0) {

        std::cout << " ** Error: no valid input file found in input directory \"" << input_dir_path <<"\"." << std::endl;
        exit(1);
    }

    return files;
}
