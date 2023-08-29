/**
 * @file value_uint.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents an unsigned 32-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_UINT_HPP__
#define __SEABOW_VALUE_UINT_HPP__

#include "values/value_int.hpp"

class SBW_ValueUint: public SBW_Value {
private:
    sbw_uint *value;

public:
};

#endif // __SEABOW_VALUE_UINT_HPP__