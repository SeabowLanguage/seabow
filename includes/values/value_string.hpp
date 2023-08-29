/**
 * @file value_string.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a list of multiple UTF-8 characters.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_STRING_HPP__
#define __SEABOW_VALUE_STRING_HPP__

#include "values/value_char.hpp"

class SBW_ValueString: public SBW_Value {
private:
    sbw_string *value;

public:
};

#endif // __SEABOW_VALUE_STRING_HPP__