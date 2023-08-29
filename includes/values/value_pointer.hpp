/**
 * @file value_pointer.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a reference to a value in memory.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_POINTER_HPP__
#define __SEABOW_VALUE_POINTER_HPP__

#include "values/value_type.hpp"

class SBW_ValuePointer: public SBW_Value {
private:
    SBW_Value *value;

public:
};

#endif // __SEABOW_VALUE_POINTER_HPP__