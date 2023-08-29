/**
 * @file value_ubyte.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow value that represents an unsigned 8-bit integer.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_UBYTE_HPP__
#define __SEABOW_VALUE_UBYTE_HPP__

#include "values/value_byte.hpp"

/**
 * @brief A seabow value that represents an unsigned 8-bit integer.
*/
class SBW_ValueUbyte : public SBW_Value {
private:
    sbw_ubyte *value; // The unsigned 8-bit integer.

public:
    /**
     * @brief Construct a new seabow valueubyte.
     * @param val The unsigned 8-bit integer.
    */
    SBW_ValueUbyte(sbw_ubyte val);

    /**
     * @brief Destroy a seabow valueubyte.
    */
    inline virtual ~SBW_ValueUbyte(sbw_none) {
        if (this->value)
            free(this->value);
    }
};

#endif // __SEABOW_VALUE_UBYTE_HPP__