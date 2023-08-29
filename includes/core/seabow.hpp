/**
 * @file seabow.hpp
 * @author LucaStarz
 * @brief Description of seabow command analyzer
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_HPP__
#define __SEABOW_HPP__

#include "interpreter.hpp"

/**
 * @brief Test if two strings are equals.
 * @param a The first string to test.
 * @param b The second string to test.
 * @return true if strings are equals, otherwise false.
*/
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

/**
 * @brief Initialize special components for seabow.
*/
sbw_none init_seabow(sbw_none);

/**
 * @brief Specify all kind of options of seabow command.
*/
enum sbw_option_type : sbw_ubyte {
    OPT_ERROR, OPT_HELP, OPT_WORD,
    OPT_INTERPRETER, OPT_COMPILER, OPT_PACKAGE_LIBRARY, OPT_BYTECODE_COMPILER, OPT_BYTECODE_INTERPRETER,

    OPT_MAIN_FILE, OPT_OUTPUT_FILE, OPT_OPTIMISATION, OPT_TARGET
};

/**
 * @brief A seabow option command.
*/
class SeabowOption {
private:
    sbw_option_type op_type; // The type of the option.
    sbw_string value; // The value of the option.

public:
    /**
     * @brief Construct a new option of seabow command without value.
     * @param ot The type of the option.
    */
    inline SeabowOption(sbw_option_type ot) { this->op_type = ot; this->value = ""; }
    
    /**
     * @brief Construct a new option of seabow command with value.
     * @param ot The type of the option.
     * @param v The value of the option.
    */
    inline SeabowOption(sbw_option_type ot, sbw_string v) { this->op_type = ot; this->value = v; }
    
    /**
     * @brief Destroy a seabow command's option.
    */
    inline virtual ~SeabowOption() {}

    /**
     * @brief Get the type of the option.
     * @return The type of the option.
    */
    inline sbw_option_type OptionType(sbw_none) { return this->op_type; }
    
    /**
     * @brief Get the value of the option.
     * @return The value of the option.
    */
    inline sbw_string Value(sbw_none) { return this->value; }
};

/**
 * @brief Get the seabow command's options.
 * @param argc The number of arguments passed to seabow command.
 * @param argv The arguments passed to seabow command.
 * @return The seabow command's options.
*/
std::vector<SeabowOption*> get_seabow_options(int argc, char **argv);

#endif // __SEABOW_HPP__