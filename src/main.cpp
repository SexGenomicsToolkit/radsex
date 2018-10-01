#include "radsex.h"
#include "mapping.h"


int main(int argc, char* argv[]) {


    /* RadSex is a class that controls the entire programme
     * This class creates one object for each type of analysis.
     * Each analysis object will has its own parser to handle its specific arguments.
     * During initialization, the main parser is first run to check the type of analysis selected,
     * then the analysis-specific parser is run to parser the analysis-specific arguments.
     */

    RadSex radsex(argc, argv);

    // If everything went well, the analysis is run
    radsex.run();

    return 0;
}
