/**
 * @file value.hpp
 * @author LucaStarz
 * @brief Description of the base of any seabow values that represents literal values.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_VALUE_HPP__
#define __SEABOW_VALUE_HPP__

#include "../base.hpp"

/**
 * @brief Specify all kind of seabow values.
*/
enum sbw_value_type : sbw_ubyte {
    SBW_VALUE_UNKNOWN = 0xff, SBW_NULL = 0xfe,

    SBW_BYTE = 0x00, SBW_UBYTE = 0x01, SBW_SHORT = 0x02, SBW_USHORT = 0x03, SBW_INT = 0x04, SBW_UINT = 0x05,
    SBW_LONG = 0x06, SBW_ULONG = 0x07, SBW_INT128 = 0x08, SBW_UINT128 = 0x09, SBW_FLOAT = 0x0a, SBW_DOUBLE = 0x0b,
    SBW_LDOUBLE = 0x0c, SBW_BOOL = 0x0d, 
    
    SBW_CHAR = 0x1a, SBW_STRING = 0x1b,

    SBW_ARRAY = 0x20, SBW_LIST = 0x21, SBW_DICT = 0x22, SBW_TUPLE = 0x23,

    SBW_ERROR = 0x30, SBW_TYPE = 0x31, SBW_POINTER = 0x32, SBW_ANY = 0x33,

    SBW_FUNCTION = 0x40, SBW_ENUMERATION = 0x41, SBW_STRUCTURE = 0x42, SBW_CLASS = 0x43
};

/**
 * @brief A seabow value that represents a literal value.
*/
class SBW_Value {
private:

public:
    /**
     * @brief Destroy a seabow value. 
    */
    inline virtual ~SBW_Value(sbw_none) {}

    /**
     * @brief Get the seabow value's type of an incorrect seabow value (i.e. SBW_VALUE_UNKNOWN).
     * @return SBW_VALUE_UNKNOWN.
    */
    inline virtual sbw_value_type Type(sbw_none) const { return SBW_VALUE_UNKNOWN; }

    /**
     * @brief <static> Get the keyword that represents a specified seabow value's type.
     * @param vt The specified seabow value's type.
     * @return The keyword that represents the specified seabow value's type.
    */
    inline static sbw_string TypeToString(sbw_value_type vt) {
        switch (vt) {
            case SBW_BYTE: return "<byte>";
            case SBW_UBYTE: return "<ubyte>";

            case SBW_NULL: return "<null>";
            default: return "<unknown>";
        }
    }
};

#endif // __SEABOW_VALUE_HPP__