/**
 * @file value_byte.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents a signed 8-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_BYTE_HPP__
#define __SEABOW_VALUE_BYTE_HPP__

#include "values/value_error.hpp"

/**
 * @brief A seabow value that represents a signed 8-bit integer.
*/
class SBW_ValueByte: public SBW_Value {
private:
    sbw_byte *value; // The signed 8-bit integer of the seabow valuebyte.

public:

    /**
     * @brief Get the seabow value's type fo a seabow valuebyte (i.e. SBW_BYTE).
     * @return SBW_BYTE.
    */
    inline sbw_value_type Type(sbw_none) const { return SBW_BYTE; }

    /**
     * @brief Check if the seabow byte is null.
     * @return true if the seabow byte is null, otherwise false.
    */
    inline sbw_bool IsNull(sbw_none) { return this->value == nullptr; }
};

#endif // __SEABOW_VALUE_BYTE_HPP__