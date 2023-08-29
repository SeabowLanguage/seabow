/**
 * @file value_dict.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a unordered dictionary.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_DICT_HPP__
#define __SEABOW_VALUE_DICT_HPP__

#include "values/value_list.hpp"

class SBW_ValueDict: public SBW_Value {
private:
    std::unordered_map<sbw_string, SBW_Value*> *value;

public:
};

#endif // __SEABOW_VALUE_DICT_HPP__