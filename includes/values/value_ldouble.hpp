/**
 * @file value_ldouble.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a 128-bit decimal number.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_LDOUBLE_HPP__
#define __SEABOW_VALUE_LDOUBLE_HPP__

#include "values/value_double.hpp"

class SBW_ValueLdouble: public SBW_Value {
private:
    sbw_ldouble *value;

public:
};

#endif // __SEABOW_VALUE_LDOUBLE_HPP__