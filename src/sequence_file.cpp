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
    std::string current_file, extension;
    std::vector<std::string> split_name;
    InputFile temp;

    while ((dir_content=readdir(dir))){

        current_file = dir_content->d_name;
        split_name = split(current_file, ".");
        size_t s = split_name.size();
        extension = "";

        // Careful: this will fail if the file name contains "." ... TODO: solve this problem
        if (s > 1) {
            for (uint i=1; i<split_name.size(); ++i) extension += "." + split_name[i];
        }

        if(std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
            temp.individual_name = split_name[0];
            temp.path = input_dir_path + current_file;
            temp.extension = extension;
            temp.processed = false;
            files.push_back(temp);
        }
    }

    return files;
}
