/**
 * @file value_double.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a 64-bit decimal number.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_DOUBLE_HPP__
#define __SEABOW_VALUE_DOUBLE_HPP__

#include "values/value_float.hpp"

class SBW_ValueDouble: public SBW_Value {
private:
    sbw_double *value;

public:
};

#endif // __SEABOW_VALUE_DOUBLE_HPP__