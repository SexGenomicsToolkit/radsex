#include "input_dir.h"

// List of currently supported file extensions
std::vector<std::string> extensions {".fq.gz", ".fq", ".fastq.gz", ".fastq", ".fasta.gz", ".fasta", ".fa.gz", ".fa", ".fna.gz", ".fna"};


std::vector<InputFile> get_input_files(Parameters& parameters) {

    struct dirent* dir_content;

    if(!parameters.input_dir) {
        exit(0);
    }

    std::vector<InputFile> files;
    std::string current_file, extension;
    std::vector<std::string> split_name;
    InputFile temp;

    while ((dir_content=readdir(parameters.input_dir))){

        current_file = dir_content->d_name;
        split_name = split(current_file, ".");
        extension = "";

        for (uint i=1; i<split_name.size(); ++i) extension += "." + split_name[i];

        if(std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
            temp.individual_name = split_name[0];
            temp.path = parameters.input_dir_path + current_file;
            temp.extension = extension;
            temp.processed = false;
            files.push_back(temp);
        }
    }

    return files;
}
