/**
 * @file value_type.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a type of a value.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_TYPE_HPP__
#define __SEABOW_VALUE_TYPE_HPP__

#include "values/value_dict.hpp"

class SBW_ValueType: public SBW_Value {
private:
    sbw_value_type type;
    sbw_string more;

public:
};

#endif // __SEABOW_VALUE_TYPE_HPP__