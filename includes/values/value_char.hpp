/**
 * @file value_char.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a UTF-32 character.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_CHAR_HPP__
#define __SEABOW_VALUE_CHAR_HPP__

#include "values/value_bool.hpp"

class SBW_ValueChar: public SBW_Value {
private:
    sbw_char *value;

public:
};

#endif // __SEABOW_VALUE_CHAR_HPP__