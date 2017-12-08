#include "arg_parser.h"


ArgParser::ArgParser(int &argc, char **argv) {

    for (auto i=1; i < argc; ++i) this->fields.push_back(std::string(argv[i]));

    if (this->contains("-h")) {
        this->usage();
        exit(0);
    }

    if (!this->contains("-i")){
        std::cout << std::endl << "** Error: no input directory specified" << std::endl;
        this->usage();
        exit(0);
    }

    if (!this->contains("-o")){
        std::cout << std::endl << "** Error: no output file specified" << std::endl;
        this->usage();
        exit(0);
    }
}



void ArgParser::set_parameters(Parameters& parameters) {

    parameters.n_threads = std::stoul(this->set_value("-t"));
    parameters.input_dir_path = std::string(realpath(this->set_value("-i").c_str(), NULL));
    parameters.output_file_path = this->set_value("-o");

    // Add a trailing "/"
    if (parameters.input_dir_path.back() != '/') parameters.input_dir_path += "/";

    //Need to check that the string given for dir path is really a dir before converting to absolute path
    parameters.input_dir = opendir(parameters.input_dir_path.c_str());

    if (!parameters.input_dir) {
        std::cout << "Error: input directory does not exist (" << parameters.input_dir_path << ")." << std::endl;
        exit(0);
    }

    parameters.output_file.open(parameters.output_file_path);
    if (not parameters.output_file.is_open()) {
        std::cout << "Error: cannot open output file (" << parameters.output_file_path << ")." << std::endl;
        exit(0);
    }
}



const std::string ArgParser::get_value(const std::string& setting) const {

    std::vector<std::string>::const_iterator itr = std::find(this->fields.begin(), this->fields.end(), setting);

    if (itr != this->fields.end() && ++itr != this->fields.end()) {

        return *itr;
    }

    return "";
}



bool ArgParser::contains(const std::string &option) const {

    return std::find(this->fields.begin(), this->fields.end(), option) != this->fields.end();
}



const std::string ArgParser::set_value(const std::string& field) {

    if (this->contains(field)) return this->get_value(field);
    else  return this->options.at(std::string(field))[0];
}



void ArgParser::usage() {

    std::cout << std::endl << "Usage: stacks_replacement -i input_dir -o output_file [-t threads]" << std::endl;
    std::cout << std::endl << "Options:" << std::endl << std::endl;
    for (auto o: this->options) std::cout << "\t" << o.first << " <" << o.second[1] << ">  " << o.second[2] << "  [" << o.second[0] << "]" << std::endl;
    std::cout << std::endl;
}


void ArgParser::print_parameters() {

    std::cout << "\n- Parameters:\n";
    for (auto o: this->options) {
        if (o.first != "-h") std::cout << "\t" << "- " << o.second[2] << " : " << this->set_value(o.first) << "\n";
    }
    std::cout << "\n";
}
