/**
 * @file value_array.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents an array of multiple seabow values.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_ARRAY_HPP__
#define __SEABOW_VALUE_ARRAY_HPP__

#include "values/value_string.hpp"

class SBW_ValueArray: public SBW_Value {
private:
    SBW_Value **value;

public:
};

#endif // __SEABOW_VALUE_ARRAY_HPP__