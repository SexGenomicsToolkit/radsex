#pragma once
#include <string>
#include "parameters.h"



struct Analysis {

    /* The Analysis structure stores information about an analysis:
     * - name of the analysis
     * - description (used for help message)
     * - parameters used in this analysis
     * It also stores a pointer to the function implementing the analysis.
     */

        std::string name = "";
        std::string description = "";
        std::vector<std::string> options;
        void (*run)(Parameters&);

        Analysis() {
            this->name = "";
            this->description = "";
            this->options = {};
            this->run = nullptr;
        }

        Analysis(std::string name, std::string description, std::vector<std::string> options, void (*run_function)(Parameters&)) {
            this->name = name;
            this->description = description;
            this->options = options;
            this->run = run_function;
        }
};
