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
     * @brief Construct a new seabow valuebyte.
     * @param val The initial signed 8-bit integer.
    */
    SBW_ValueByte(sbw_byte val);

    /**
     * @brief Destroy a seabow valuebyte.
    */
    inline virtual ~SBW_ValueByte(sbw_none) {
        if (this->value)
            free(this->value);
    }

    /**
     * @brief Get the signed 8-bit value.
     * @return The signed 8-bit value.
    */
    inline sbw_byte Get(sbw_none) const { return *this->value; }

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

    /**
     * @brief Perform an unary plus operation on a byte value.
     * @return A copy of the byte value.
    */
    SBW_Value *unary_plus(sbw_none);

    /**
     * @brief Perform a binary plus operaton on a byte value and another value.
     * @param other The another value.
     * @return The result of the operation.
    */
    SBW_Value *plus(SBW_Value *other);

    /**
     * @brief Perform a left incrementation operation on a byte value.
     * @return The byte value incremented.
    */
    SBW_Value *left_incr(sbw_none);

    /**
     * @brief Perform a right incrementation operation on a byte value.
     * @return A copy of the byte value.
    */
    SBW_Value *right_incr(sbw_none);

    /**
     * @brief Perform a plus equals operation on a byte value and another value.
     * @param other The another value.
     * @return The new value of the byte value.
    */
    SBW_Value *plus_equals(SBW_Value *other);
};

#endif // __SEABOW_VALUE_BYTE_HPP__