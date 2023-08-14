#ifndef __SEABOW_VALUE_HPP__
#define __SEABOW_VALUE_HPP__

#include "core/lexer.hpp"

enum sbw_value_type : sbw_ubyte {
    SBW_VALUE_UNKNOWN = 0xff, SBW_NULL = 0xfe,

    SBW_BYTE = 0x00, SBW_UBYTE = 0x01, SBW_SHORT = 0x02, SBW_USHORT = 0x03, SBW_INT = 0x04, SBW_UINT = 0x05,
    SBW_LONG = 0x06, SBW_ULONG = 0x07, SBW_INT128 = 0x08, SBW_UINT128 = 0x09, SBW_FLOAT = 0x0a, SBW_DOUBLE = 0x0b,
    SBW_LDOUBLE = 0x0c, SBW_BOOL = 0x0d, 
    
    SBW_CHAR = 0x1a, SBW_STRING = 0x1b,

    SBW_ARRAY = 0x20, SBW_LIST = 0x21, SBW_DICT = 0x22, SBW_TUPLE = 0x23,

    SBW_ERROR = 0x30, SBW_TYPE = 0x31, SBW_ANY = 0x32,

    SBW_FUNCTION = 0x40
};

class SBW_Value {
private:
    sbw_value_type vtype;

public:
    inline SBW_Value(sbw_none) { this->vtype = SBW_VALUE_UNKNOWN; }
    inline virtual ~SBW_Value(sbw_none) {}

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