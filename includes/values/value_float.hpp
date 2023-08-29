/**
 * @file value_float.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a 32-bit decimal number.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_FLOAT_HPP__
#define __SEABOW_VALUE_FLOAT_HPP__

#include "values/value_uint128.hpp"

class SBW_ValueFloat: public SBW_Value {
private:
    sbw_float *value;

public:
};

#endif // __SEABOW_VALUE_FLOAT_HPP__