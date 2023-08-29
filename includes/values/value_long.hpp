/**
 * @file value_long.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a signed 64-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_LONG_HPP__
#define __SEABOW_VALUE_LONG_HPP__

#include "value_uint.hpp"

class SBW_ValueLong: public SBW_Value {
private:
    sbw_long *value;

public:
};

#endif // __SEABOW_VALUE_LONG_HPP__