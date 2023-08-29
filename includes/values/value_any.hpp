/**
 * @file value_any.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents any kind of value.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_ANY_HPP__
#define __SEABOW_VALUE_ANY_HPP__

#include "values/value_class.hpp"

class SBW_ValueAny: public SBW_Value {
private:
    SBW_Value *value;

public:
};

#endif // __SEABOW_VALUE_ANY_HPP__