#pragma once
#include <string>
#include <fstream>
#include <dirent.h>



struct Parameter {

    /* The Parameter structure stores the information necessary to use and process the parameters
     * This structure also offers a few methods to process parameter values
     */


    // Properties of a parameter object
    std::string name;
    std::string description;
    std::string flag;
    std::string default_value;
    std::string type; // Type to be displayed in help message
    std::string internal_type; // Type to be used for internal value testing
    std::string value;
    bool required; // Whether the parameter value has to be specified by user or a default value can be used


    // Constructor: set the values of the different properties
    Parameter(std::string const& name = "", std::string const& description = "", std::string const& flag = "", std::string const& default_value = "",
              std::string const& type = "", std::string const& internal_type = "", std::string const& value = "", bool required = false) {

        this->name = name;
        this->description = description;
        this->flag = flag;
        this->default_value = default_value;
        this->type = type;
        this->internal_type = internal_type;
        this->required = required;
        if (value != "") this->value = value; else this->value = default_value;
    }


    // Tests if the parameter was initialized or was just returned as a null (dummy) parameter by a function
    bool is_real() {
        if (this->name == "" and this->flag == "" and this->type == "" and this->value == "") return false;
        return true;
    }


    // Tests if the value of the parameter matches its type
    bool test_value() {
        if (this->internal_type == "bool") {
            if (this->value == "0" or this->value == "1") return true; else return false;
        } else if (this->internal_type == "int") {
            return this->value.find_first_not_of( "0123456789" ) == std::string::npos;
        } else if (this->internal_type == "ifile") {
            std::ifstream file;
            file.open(this->value);
            return file.is_open();
        } else if (this->internal_type == "ofile") {
            std::ofstream file;
            file.open(this->value);
            return file.is_open();
        } else if (this->internal_type == "dir") {
            DIR* dir = opendir(this->value.c_str());
            return (dir != NULL);
        }else {
            return true;
        }
    }


    // Sets the value of the parameter. Special check for directory path.
    void set_value(std::string& value) {
        if (this->internal_type == "dir" and value.back() != '/') value += "/";
        this->value = value;
    }

};





