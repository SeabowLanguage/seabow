/**
 * @file value_uint128.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents an unsigned 128-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_UINT128_HPP__
#define __SEABOW_VALUE_UINT128_HPP__

#include "values/value_int128.hpp"

class SBW_ValueUint128: public SBW_Value {
private:
    sbw_uint128 *value;

public:
};

#endif // __SEABOW_VALUE_UINT128_HPP__