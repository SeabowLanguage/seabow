/**
 * @file value_ushort.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents an unsigned 16-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_USHORT_HPP__
#define __SEABOW_VALUE_USHORT_HPP__

#include "values/value_short.hpp"

class SBW_ValueUshort: public SBW_Value {
private:
    sbw_ushort *value;

public:

    inline sbw_bool IsNull(sbw_none) { return this->value == nullptr; }
};

#endif // __SEABOW_VALUE_USHORT_HPP__