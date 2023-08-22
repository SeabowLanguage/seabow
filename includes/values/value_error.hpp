#ifndef __SEABOW_VALUE_ERROR_HPP__
#define __SEABOW_VALUE_ERROR_HPP__

#include "value.hpp"

class SBW_ValueError: public SBW_Value {
private:
    sbw_string name, details;
    sbw_ulong line, column;

public:
    SBW_ValueError(sbw_string n, sbw_string d, sbw_ulong l, sbw_ulong c);
    SBW_ValueError(sbw_string n, sbw_string d);
    inline virtual ~SBW_ValueError(sbw_none) {}

    inline sbw_value_type Type(sbw_none) const { return SBW_ERROR; }
    inline sbw_string Name(sbw_none) { return this->name; }
    inline sbw_string Details(sbw_none) { return this->details; }
    inline sbw_ulong Line(sbw_none) { return this->line; }
    inline sbw_ulong Column(sbw_none) { return this->column; }

    inline sbw_none SetPos(sbw_ulong l, sbw_ulong c) { this->line = l; this->column = c; }
};

#endif // __SEABOW_VALUE_ERROR_HPP__