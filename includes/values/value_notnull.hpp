/**
 * @file value_notnull.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a non nullable value.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_NOTNULL_HPP__
#define __SEABOW_VALUE_NOTNULL_HPP__

#include "values/value_null.hpp"

class SBW_ValueNotNull: public SBW_Value {
private:
    SBW_Value *value;

public:

    inline sbw_bool IsNull(sbw_none) { return false; }
};

#endif // __SEABOW_VALUE_NOTNULL_HPP__