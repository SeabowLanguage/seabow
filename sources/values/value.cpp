#include "values/value_error.hpp"

SBW_Value *SBW_Value::UnaryOpError(sbw_string op, sbw_value_type type)
{    
    return new SBW_ValueError("OperatorError", "Unary operator '" + op + "' is not defined for type " + SBW_Value::TypeToString(type));
}

SBW_Value *SBW_Value::BinaryOpError(sbw_string op, sbw_value_type t1, sbw_value_type t2)
{
    return new SBW_ValueError("OperatorError", "Binary operator '" + op + "' is not defined for types " + SBW_Value::TypeToString(t1) + " and " + SBW_Value::TypeToString(t2));
}

SBW_Value *SBW_Value::NullOpError(sbw_string op)
{
    return new SBW_ValueError("OperatorError", "Can not use operator '" + op + "' with null value");
}

SBW_Value *SBW_Value::ConversionError(sbw_value_type from, sbw_value_type to)
{
    return new SBW_ValueError("ConversionError", "Can not convert from type " + SBW_Value::TypeToString(from) + " to type " + SBW_Value::TypeToString(to));
}

SBW_Value *SBW_Value::AutoConversionError(sbw_value_type from, sbw_value_type to)
{
    return new SBW_ValueError("ConversionError", "Can not automatically convert from type " + SBW_Value::TypeToString(from) + " to type " + SBW_Value::TypeToString(to));
}

SBW_Value *SBW_Value::ZeroDivisionError(sbw_none)
{
    return new SBW_ValueError("ZeroDivisionError", "Can not divide by zero");
}

SBW_Value *SBW_Value::unary_plus(sbw_none)
{
    return SBW_Value::UnaryOpError("+", this->Type());
}

SBW_Value *SBW_Value::plus(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("+", this->Type(), other->Type());
}

SBW_Value *SBW_Value::left_incr(sbw_none)
{
    return SBW_Value::UnaryOpError("left ++", this->Type());
}

SBW_Value *SBW_Value::right_incr(sbw_none)
{
    return SBW_Value::UnaryOpError("right ++", this->Type());
}

SBW_Value *SBW_Value::plus_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("+=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::unary_minus(sbw_none)
{
    return SBW_Value::UnaryOpError("-", this->Type());
}

SBW_Value *SBW_Value::minus(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("-", this->Type(), other->Type());
}

SBW_Value *SBW_Value::left_decr(sbw_none)
{
    return SBW_Value::UnaryOpError("left --", this->Type());
}

SBW_Value *SBW_Value::right_decr(sbw_none)
{
    return SBW_Value::UnaryOpError("right --", this->Type());
}

SBW_Value *SBW_Value::minus_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("-=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::times(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("*", this->Type(), other->Type());
}

SBW_Value *SBW_Value::times_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("*=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::div(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("/", this->Type(), other->Type());
}

SBW_Value *SBW_Value::div_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("/=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::rem(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("%", this->Type(), other->Type());
}

SBW_Value *SBW_Value::rem_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("%=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::bitwise_xor(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("^", this->Type(), other->Type());
}

SBW_Value *SBW_Value::bitwise_xor_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("^=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::bitwise_and(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("&", this->Type(), other->Type());
}

SBW_Value *SBW_Value::bitwise_and_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("&=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::bitwise_or(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("|", this->Type(), other->Type());
}

SBW_Value *SBW_Value::bitwise_or_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("|=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::bitwise_not(sbw_none)
{
    return SBW_Value::UnaryOpError("~", this->Type());
}

SBW_Value *SBW_Value::assign(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::get(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("[]", this->Type(), other->Type());
}

SBW_Value *SBW_Value::set(SBW_Value *index, SBW_Value *other)
{
    SBW_Value *err = this->get(index);
    if (err->Type() != SBW_ERROR)
        err = SBW_Value::BinaryOpError("[] =", this->Type(), other->Type());
    return err;
}

SBW_Value *SBW_Value::left_shift(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("<<", this->Type(), other->Type());
}

SBW_Value *SBW_Value::left_shift_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("<<=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::right_shift(SBW_Value *other)
{
    return SBW_Value::BinaryOpError(">>", this->Type(), other->Type());
}

SBW_Value *SBW_Value::right_shift_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError(">>=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::_not(sbw_none)
{
    return SBW_Value::UnaryOpError("!", this->Type());
}

SBW_Value *SBW_Value::equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("==", this->Type(), other->Type());
}

SBW_Value *SBW_Value::not_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("!=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::less(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("<", this->Type(), other->Type());
}

SBW_Value *SBW_Value::less_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("<=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::great(SBW_Value *other)
{
    return SBW_Value::BinaryOpError(">", this->Type(), other->Type());
}

SBW_Value *SBW_Value::great_equals(SBW_Value *other)
{
    return SBW_Value::BinaryOpError(">=", this->Type(), other->Type());
}

SBW_Value *SBW_Value::_and(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("&&", this->Type(), other->Type());
}

SBW_Value *SBW_Value::_or(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("||", this->Type(), other->Type());
}

SBW_Value *SBW_Value::ref(sbw_none)
{
    return SBW_Value::UnaryOpError("$", this->Type());
}

SBW_Value *SBW_Value::contains(SBW_Value *other)
{
    return SBW_Value::BinaryOpError("in", this->Type(), other->Type());   
}

SBW_Value *SBW_Value::convert(sbw_value_type to)
{
    return SBW_Value::ConversionError(this->Type(), to);
}

SBW_Value *SBW_Value::is(sbw_value_type to)
{
    return SBW_Value::BinaryOpError("is", this->Type(), to);
}

SBW_Value *SBW_Value::for_each(sbw_none)
{
    return SBW_Value::UnaryOpError("left in", this->Type());
}

SBW_Value *SBW_Value::AutoConvert(sbw_value_type to)
{
    return SBW_Value::AutoConversionError(this->Type(), to);
}