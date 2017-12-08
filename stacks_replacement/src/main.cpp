#include "arg_parser.h"
#include "analysis.h"


int main(int argc, char *argv[]) {

    Parameters parameters;

    ArgParser cmd_options(argc, argv);

    cmd_options.set_parameters(parameters);
    cmd_options.print_parameters();

    analysis(parameters);

    closedir(parameters.input_dir);

    return 0;
}
