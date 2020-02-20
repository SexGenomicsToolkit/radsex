#include "utils.h"

const char revcomp_table[] = {
      0,   1,	2,	 3,	  4,   5,	6,	 7,	  8,   9,  10,	11,	 12,  13,  14,	15,
     16,  17,  18,	19,	 20,  21,  22,	23,	 24,  25,  26,	27,	 28,  29,  30,	31,
     32,  33,  34,	35,	 36,  37,  38,	39,	 40,  41,  42,	43,	 44,  45,  46,	47,
     48,  49,  50,	51,	 52,  53,  54,	55,	 56,  57,  58,	59,	 60,  61,  62,	63,
     64, 'T', 'V', 'G', 'H', 'E', 'F', 'C', 'D', 'I', 'J', 'M', 'L', 'K', 'N', 'O',
    'P', 'Q', 'Y', 'S', 'A', 'A', 'B', 'W', 'X', 'R', 'Z',	91,	 92,  93,  94,	95,
     64, 't', 'v', 'g', 'h', 'e', 'f', 'c', 'd', 'i', 'j', 'm', 'l', 'k', 'n', 'o',
    'p', 'q', 'y', 's', 'a', 'a', 'b', 'w', 'x', 'r', 'z', 123, 124, 125, 126, 127
};



// Output current date and time in format specified in utils.h
char* print_time (char *buff) {

    time_t t = time (nullptr);
    strftime (buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}



// Splits a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter){

    std::vector<std::string> output;
    size_t pos;

    while ((pos = str.find(delimiter)) != std::string::npos){

        output.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }

    output.push_back(str.substr(0, pos));

    return output;
}


void rev_comp(const std::string& sequence, std::string& revcomp_sequence) {

    revcomp_sequence = "";
    for (auto it = sequence.rbegin(); it != sequence.rend(); ++it) {
        revcomp_sequence += revcomp_table[int(*it)];
    }
}



std::vector<std::string> get_column_sex(std::unordered_map<std::string, std::string>& popmap, const std::vector<std::string>& header) {

    // Map with column number --> index of sex_count (0 = male, 1 = female, 2 = no sex)
    std::vector<std::string> sex_columns;

    for (uint i=0; i<header.size(); ++i) {

        if (popmap.find(header[i]) != popmap.end()) {

            sex_columns.push_back(popmap[header[i]]);

        } else {

            sex_columns.push_back(""); // First and second columns (id and sequence) are not counted

        }
    }

    return sex_columns;
}

