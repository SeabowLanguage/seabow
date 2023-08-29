#include "values/value_notnull.hpp"

SBW_ValueUbyte::SBW_ValueUbyte(sbw_ubyte val)
{
    this->value = static_cast<sbw_ubyte*>(malloc(sizeof(sbw_ubyte)));
    if (this->value)
        *this->value = val;
}