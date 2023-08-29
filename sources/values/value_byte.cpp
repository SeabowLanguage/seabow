#include "values/value_notnull.hpp"

SBW_ValueByte::SBW_ValueByte(sbw_byte val)
{
    this->value = static_cast<sbw_byte*>(malloc(sizeof(sbw_byte)));
    if (this->value)
        *this->value = val;
}

SBW_Value *SBW_ValueByte::unary_plus(sbw_none)
{
    if (this->IsNull())
        return SBW_Value::NullOpError("+");
    
    return new SBW_ValueByte(*this->value);
}

SBW_Value *SBW_ValueByte::plus(SBW_Value *other)
{
    
}

SBW_Value *SBW_ValueByte::left_incr(sbw_none)
{
    if (this->IsNull())
        return SBW_Value::NullOpError("left ++");
    
    *this->value += 1;
    return new SBW_ValueByte(*this->value);
}

SBW_Value *SBW_ValueByte::right_incr(sbw_none)
{
    if (this->IsNull())
        return SBW_Value::NullOpError("right ++");
    
    sbw_byte old = *this->value;
    *this->value += 1;
    return new SBW_ValueByte(old);
}

SBW_Value *SBW_ValueByte::plus_equals(SBW_Value *other)
{

}