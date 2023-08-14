#include "values/value_error.hpp"

SBW_ValueError::SBW_ValueError(sbw_string n, sbw_string d, sbw_ulong l, sbw_ulong c)
{
    this->name = n;
    this->details = d;
    this->line = l;
    this->column = c;
}

SBW_ValueError::SBW_ValueError(sbw_string n, sbw_string d)
{
    this->name = n;
    this->details = d;
    this->line = this->column = 0;
}