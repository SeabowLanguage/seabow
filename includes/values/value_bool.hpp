/**
 * @file value_bool.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a boolean value.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_BOOL_HPP__
#define __SEABOW_VALUE_BOOL_HPP__

#include "values/value_ldouble.hpp"

class SBW_ValueBool: public SBW_Value {
private:
    sbw_bool *value;

public:
};

#endif // __SEABOW_VALUE_BOOL_HPP__