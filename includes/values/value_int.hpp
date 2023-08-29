/**
 * @file value_int.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a signed 32-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_INT_HPP__
#define __SEABOW_VALUE_INT_HPP__

#include "values/value_ushort.hpp"

class SBW_ValueInt: public SBW_Value {
private:
    sbw_int *value;

public:
};

#endif // __SEABOW_VALUE_INT_HPP__