/**
 * @file value_short.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a signed 16-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_SHORT_HPP__
#define __SEABOW_VALUE_SHORT_HPP__

#include "values/value_ubyte.hpp"

class SBW_ValueShort: public SBW_Value {
private:
    sbw_short *value;

public:

    inline sbw_bool IsNull(sbw_none) { return this->value == nullptr; }
};

#endif // __SEABOW_VALUE_SHORT_HPP__