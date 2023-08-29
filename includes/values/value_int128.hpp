/**
 * @file value_int128.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a signed 128-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_INT128_HPP__
#define __SEABOW_VALUE_INT128_HPP__

#include "values/value_ulong.hpp"

class SBW_ValueInt128: public SBW_Value {
private:
    sbw_int128 *value;

public:
};

#endif // __SEABOW_VALUE_INT128_HPP__