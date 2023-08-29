/**
 * @file value_list.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a resizable list of multiple values.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_LIST_HPP__
#define __SEABOW_VALUE_LIST_HPP__

#include "value_array.hpp"

class SBW_ValueList: public SBW_Value {
private:
    std::vector<SBW_Value*> *value;

public:
};

#endif // __SEABOW_VALUE_LIST_HPP__