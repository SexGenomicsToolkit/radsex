#include "parameters.h"


/* The function "get_value_from_name" requires template specialization.
 * Strings are converted to the specified type using stringstreams, which are "cut" by spaces.
 * The below function specializes the function for values of type "string" in order to handle strings containing spaces
 */

template<>
std::string Parameters::get_value_from_name<std::string>(const std::string& name) {
    for (auto p: this->list) {
        if (p.name == name) {
            return p.value;
        }
    }
    std::cout << " ** Error: looking for the value of parameter \"" << name <<"\" which does not exist." << std::endl;
    exit(0);
}
