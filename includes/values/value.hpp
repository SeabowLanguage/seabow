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

    /**
     * @brief <static> Create a seabow error for undefined relation between an unary operator and a seabow value type.
     * @param op The unary operator.
     * @param type The seabow value type.
     * @return The seabow error.
    */
    static SBW_Value *UnaryOpError(sbw_string op, sbw_value_type type);
    
    /**
     * @brief <static> Create a seabow error for undefined relation bewteen a binary operator and two seabow value types.
     * @param op The binary operator.
     * @param t1 The first seabow value type.
     * @param t2 The second seabow value type.
     * @return The seabow error.
    */
    static SBW_Value *BinaryOpError(sbw_string op, sbw_value_type t1, sbw_value_type t2);
    
    /**
     * @brief <static> Create a seabow error for undefined relation between an operator and a null value.
     * @param op The operator.
     * @return The seabow error.
    */
    static SBW_Value *NullOpError(sbw_string op);
    
    /**
     * @brief <static> Create a seabow error for undefined conversion between two seabow value types.
     * @param from The seabow value type of the value to convert.
     * @param to The seabow value type of the converted value.
     * @return The seabow error.
    */
    static SBW_Value *ConversionError(sbw_value_type from, sbw_value_type to);
    
    /**
     * @brief <static> Create a seabow error for undefined automatic conversion between two seabow value types.
     * @param from The seabow value type of the value to convert.
     * @param to The seabow value type of the converted value.
     * @return The seabow error.
    */
    static SBW_Value *AutoConversionError(sbw_value_type from, sbw_value_type to);

    /**
     * @brief <static> Create a seabow error for zero division operation error.
     * @return The seabow error.
    */
    static SBW_Value *ZeroDivisionError(sbw_none);

    /**
     * @brief Perform an unary plus operation on a seabow value.
     * @return A seabow error for undefined operator 'unary +'.
    */
    virtual SBW_Value *unary_plus(sbw_none);
    
    /**
     * @brief Perform a binary plus operator on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary +'.
    */
    virtual SBW_Value *plus(SBW_Value *other);
    
    /**
     * @brief Perform a left incrementation on a seabow value.
     * @return A seabow error for undefined operator 'unary left ++'.
    */
    virtual SBW_Value *left_incr(sbw_none);
    
    /**
     * @brief Perform a right incrementation on a seabow value.
     * @return A seabow error for undefined operator 'unary right ++'.
    */
    virtual SBW_Value *right_incr(sbw_none);

    /**
     * @brief Perform a binary plus equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary +='.
    */
    virtual SBW_Value *plus_equals(SBW_Value *other);

    /**
     * @brief Perform an unary minus operation on a seabow value.
     * @return A seabow error for undefined operator 'unary -'.
    */
    virtual SBW_Value *unary_minus(sbw_none);

    /**
     * @brief Perform a binary minus operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary -'.
    */
    virtual SBW_Value *minus(SBW_Value *other);

    /**
     * @brief Perform a left decrementation on a seabow value.
     * @return A seabow error for undefined operator 'unary left --'.
    */
    virtual SBW_Value *left_decr(sbw_none);

    /**
     * @brief Perform an right decrementation on a seabow value.
     * @return A seabow error for undefined operator 'unary right --'.
    */
    virtual SBW_Value *right_decr(sbw_none);

    /**
     * @brief Perform a binary minus equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary -='.
    */
    virtual SBW_Value *minus_equals(SBW_Value *other);

    /**
     * @brief Perform a binary times operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary *'.
    */
    virtual SBW_Value *times(SBW_Value *other);

    /**
     * @brief Perform a binary times equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary *='.
    */
    virtual SBW_Value *times_equals(SBW_Value *other);

    /**
     * @brief Perform a binary division operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary /'.
    */
    virtual SBW_Value *div(SBW_Value *other);

    /**
     * @brief Perform a binary division equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary /='.
    */
    virtual SBW_Value *div_equals(SBW_Value *other);

    /**
     * @brief Perform a binary modulo operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary %'.
    */
    virtual SBW_Value *rem(SBW_Value *other);

    /**
     * @brief Perform a binary modulo equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary %='.
    */
    virtual SBW_Value *rem_equals(SBW_Value *other);

    /**
     * @brief Perform a binary bitwise exclusive or operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary ^'.
    */
    virtual SBW_Value *bitwise_xor(SBW_Value *other);

    /**
     * @brief Perform a binary bitwise eclusive or equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary ^='.
    */
    virtual SBW_Value *bitwise_xor_equals(SBW_Value *other);

    /**
     * @brief Perform a binary bitwise and operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary &'.
    */
    virtual SBW_Value *bitwise_and(SBW_Value *other);

    /**
     * @brief Perform a binary bitwise and equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary &='.
    */
    virtual SBW_Value *bitwise_and_equals(SBW_Value *other);

    /**
     * @brief Perform a binary bitwise or operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary |'.
    */
    virtual SBW_Value *bitwise_or(SBW_Value *other);

    /**
     * @brief Perform a binary bitwise or equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary |='.
    */
    virtual SBW_Value *bitwise_or_equals(SBW_Value *other);

    /**
     * @brief Perform an unary bitwise not operation on a seabow value.
     * @return A seabow error for undefined operator 'unary ~'.
    */
    virtual SBW_Value *bitwise_not(sbw_none);

    /**
     * @brief Perform a binary assign operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary ='.
    */
    virtual SBW_Value *assign(SBW_Value *other);

    /**
     * @brief Perform a binary get operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary []'.
    */
    virtual SBW_Value *get(SBW_Value *other);

    /**
     * @brief Perform a binary set operation on three seabow values.
     * @param index The index value of the set operation.
     * @param other The other value of the get operation.
     * @return A seabow error for undefined operator 'binary [] ='.
    */
    virtual SBW_Value *set(SBW_Value *index, SBW_Value *other);

    /**
     * @brief Perform a binary left shift operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary <<'.
    */
    virtual SBW_Value *left_shift(SBW_Value *other);

    /**
     * @brief Perform a binary left shift equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary <<='.
    */
    virtual SBW_Value *left_shift_equals(SBW_Value *other);

    /**
     * @brief Perform a binary right shift equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary >>'.
    */
    virtual SBW_Value *right_shift(SBW_Value *other);

    /**
     * @brief Perform a binary right shift equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary >>='.
    */
    virtual SBW_Value *right_shift_equals(SBW_Value *other);

    /**
     * @brief Perform an unary not operation on a seabow value.
     * @return A seabow error for undefined operator 'unary !'.
    */
    virtual SBW_Value *_not(sbw_none);

    /**
     * @brief Perform a binary equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary =='.
    */
    virtual SBW_Value *equals(SBW_Value *other);

    /**
     * @brief Perform a binary not equals operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary !='.
    */
    virtual SBW_Value *not_equals(SBW_Value *other);

    /**
     * @brief Perform a binary less than operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary <'.
    */
    virtual SBW_Value *less(SBW_Value *other);

    /**
     * @brief Perform a binary less or equals than operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary <='.
    */
    virtual SBW_Value *less_equals(SBW_Value *other);

    /**
     * @brief Perform a binary greater than operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary >'.
    */
    virtual SBW_Value *great(SBW_Value *other);

    /**
     * @brief Perform a binary greater or equals than operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary >='.
    */
    virtual SBW_Value *great_equals(SBW_Value *other);

    /**
     * @brief Perform a binary and operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary &&'.
    */
    virtual SBW_Value *_and(SBW_Value *other);

    /**
     * @brief Perform a binary or operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary ||'.
    */
    virtual SBW_Value *_or(SBW_Value *other);

    /**
     * @brief Perform an unary reference of operation on a seabow value.
     * @return A seabow error for undefined operator 'binary $'.
    */
    virtual SBW_Value *ref(sbw_none);

    /**
     * @brief Perform a binary contains operation on two seabow values.
     * @param other The second seabow value.
     * @return A seabow error for undefined operator 'binary in'.
    */
    virtual SBW_Value *contains(SBW_Value *other);

    /**
     * @brief Perform a binary conversion operation on a seabow value.
     * @param to The destination type for the conversion.
     * @return A seabow error for undefined conversion.
    */
    virtual SBW_Value *convert(sbw_value_type to);

    /**
     * @brief Perform a binary is operation on a seabow value.
     * @param to The check type for the is operation.
     * @return A seabow error for undefined operator 'binary is'.
    */
    virtual SBW_Value *is(sbw_value_type to);

    /**
     * @brief Perform a foreach listing operation on a seabow value.
     * @return A seabow error for undefined operator 'left in'.
    */
    virtual SBW_Value *for_each(sbw_none);

    /**
     * @brief Perform an automatic conversion operation on a seabow value.
     * @param to The destination type for the conversion.
     * @return A seabow error for undefined automatic conversion.
    */
    virtual SBW_Value *AutoConvert(sbw_value_type to);

    /**
     * @brief Check if a seabow value is null.
     * @return true if the seabow value is null, otherwise false.
    */
    virtual sbw_bool IsNull(sbw_none) = 0;
};

#endif // __SEABOW_VALUE_HPP__