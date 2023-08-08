/**
 * @file seabow.hpp
 * @author LucaStarz
 * @brief 
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_HPP__
#define __SEABOW_HPP__

#include "lexer.hpp"

inline sbw_bool string_equals(const char *a, const char *b) {
    sbw_ulong size = 0;
    while (a[size] != '\0' && b[size] != '\0')
        size++;
    
    if (a[size] != '\0' || b[size] != '\0')
        return false;
    
    for (sbw_ulong i=0; i<size; i++) {
        if (a[i] != b[i])
            return false;
    }

    return true;
}

sbw_none init_seabow(sbw_none);

enum sbw_option_type : sbw_ubyte {
    OPT_ERROR, OPT_HELP, OPT_WORD,
    OPT_INTERPRETER, OPT_COMPILER, OPT_PACKAGE_LIBRARY, OPT_BYTECODE_COMPILER, OPT_BYTECODE_INTERPRETER,
    OPT_INSTALL, OPT_PUBLISH,

    OPT_MAIN_FILE, OPT_OUTPUT_FILE, OPT_OPTIMISATION, OPT_TARGET
};

class SeabowOption {
private:
    sbw_option_type op_type;
    sbw_string value;

public:
    inline SeabowOption(sbw_option_type ot) { this->op_type = ot; this->value = ""; }
    inline SeabowOption(sbw_option_type ot, sbw_string v) { this->op_type = ot; this->value = v; }
    inline virtual ~SeabowOption() {}

    inline sbw_option_type OptionType(sbw_none) { return this->op_type; }
    inline sbw_string Value(sbw_none) { return this->value; }
};

std::vector<SeabowOption*> get_seabow_options(int argc, char **argv);

#endif // __SEABOW_HPP__