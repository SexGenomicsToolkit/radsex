/*
* Copyright (C) 2020 Romain Feron
* This file is part of RADSex.

* RADSex is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* RADSex is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with RADSex.  If not, see <https://www.gnu.org/licenses/>.
*/

/*!
 * @file arg_parser.h
 * @brief Implements CLI11 overriding classes for CLI parsing.
*/

#pragma once
#include <iostream>
#include <stdio.h>
#include "CLI11/CLI11.hpp"
#include "parameters.h"
#include "utils.h"


/*!
 * \brief Print CLI parsing failure message
 *
 * Identify the type of error (missing subcommand, argument error, or other)
 * and display the appropriate error message.
 *
 * \param parser Pointer to a CLI::App instance
 * \param error  CLI::Error object with information on the error
 *
 * \return A string containing the error message
 */

inline std::string failure_message(const CLI::App* parser, const CLI::Error& error) {

    std::string message = "";

    if (error.what() == std::string("A subcommand is required")) {

        message = "\nSubcommand error: missing or invalid subcommand\n\n" + parser->help();

    } else if (error.get_exit_code() == 106) {  // 106 corresponds to wrong argument type

        message = "\nArgument error: " + std::string(error.what()) + "\n\n" + parser->help();

    } else {

        message = "\nError: " + std::string(error.what()) + "\n\n" + parser->help();

    }

    return message;
}



/*!
 * \brief Custom CLI11 Formatter
 *
 * This class is used to override default help message format in CLI11 Formatter.
 */

class CustomFormatter : public CLI::Formatter {

    public:

        uint column_widths[3] {0, 0, 0};  ///< Maximum width of each column, in order: flags, type, description
        uint border_width = 4;  ///< Define number of spaces between two columns


        /*!
         * \brief Format help message for an option
         *
         * Generate a nicely formatted help message for an option, using column widths to align columns
         * for all options properly.
         *
         * \param opt            Pointer to an CLI::Option instance
         * \param is_positional  Boolean indicating whether the option is positional
         *
         * \return A string containing the formatted help message for the option
         */

        virtual std::string make_option(const CLI::Option* opt, const bool is_positional) const {

            std::string option = "", name = "", type = "", description = "", default_value = "", required = "REQUIRED", short_name = "";
            std::vector<std::string> options;

            // Generate option name
            if (is_positional) {

                name = opt->get_name();  // Positional: use the option's name

            } else {

                // Not positional: name is <short_flag, long_flag>
                options = split(opt->get_name(false, true), ",");  // Get all flags
                options.size() == 1 ? name = options[0] : name = options[1] + ", " + options[0];  // If only one flag, use it, else use <short, long>

            }

            // Get option info
            type = opt->get_type_name();
            description = opt->get_description();
            default_value = opt->get_default_str();

            // Generate the help message for this option, adding the right number of spaces after each column based on column_widths
            option = name + std::string(border_width + column_widths[0] - name.size(), ' ');
            option += type + std::string(border_width + column_widths[1] - type.size(), ' ');
            option += description + std::string(border_width + column_widths[2] - description.size(), ' ');
            if (opt->get_required()) default_value = required;
            if (default_value != "") option += "[" + default_value + "]";
            option += "\n";

            return option;
        }


        /*!
         * \brief Format command description
         *
         * Remove command description in CLI help message.
         * \return An empty string
         */

        virtual std::string make_description(const CLI::App*) const {

            return "";
        }


        /*!
         * \brief Set column widths in help message for a CLI::App
         *
         * Determine column widths from the maximum element length in each column
         *
         * \param parser Pointer to a CLI::App instance
         */

        void set_column_widths(const CLI::App& parser) {

            std::string tmp = "";

            for (auto opt: parser.get_subcommands()[0]->get_options()) {

                tmp = opt->get_name();
                if (tmp.size() > this->column_widths[0]) this->column_widths[0] = static_cast<uint>(tmp.size());

                tmp = opt->get_type_name();
                if (tmp.size() > this->column_widths[1]) this->column_widths[1] = static_cast<uint>(tmp.size());

                tmp = opt->get_description();
                if (tmp.size() > this->column_widths[2]) this->column_widths[2] = static_cast<uint>(tmp.size());

            }

            this->column_widths[0] += 2;
        }

};
