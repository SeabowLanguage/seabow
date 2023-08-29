/**
 * @file token.hpp
 * @author LucaStarz
 * @brief Description of a seabow token (i.e. a part of code).
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_TOKEN_HPP__
#define __SEABOW_TOKEN_HPP__

#include "values/value_error.hpp"

/**
 * @brief Specify all kind of seabow tokens.
*/
enum sbw_token_type: sbw_ubyte {
    TT_BAD = 0xff, TT_EOF = 0xfe, 
    TT_NEW_LINE = 0xfd, // Use for separate expressions (like ';')

    TT_IN = 0xfd, TT_IS = 0xfc, TT_AS = 0xfb, TT_NULL = 0xfa,

    TT_WORD = 0x00, TT_CHAR = 0x01, TT_STRING = 0x02, TT_INTEGER = 0x03, TT_DECIMAL = 0x04, TT_BOOLEAN = 0x05,
    TT_LONG_INTEGER = 0x06, TT_LONG_DECIMAL = 0x07, TT_CONTROLLER = 0x08, TT_HEXADECIMAL = 0x09, TT_OCTAL = 0x0a, TT_BINARY = 0x0b,

    TT_LPAR = 0x10, TT_RPAR = 0x11, TT_LBRACE = 0x12, TT_RBRACE = 0x13, TT_LBRACKET = 0x14, TT_RBRACKET = 0x15,
    TT_SEMI = 0x16, TT_COMMA = 0x17, TT_COLON = 0x18, TT_DOT = 0x19,
    TT_QUEST = 0x1a, TT_DOLLAR = 0x1b, TT_AT = 0x1c, TT_HASH = 0x1d,

    TT_EQ = 0x30, TT_EE = 0x31, TT_NOT = 0x32, TT_NE = 0x33,
    TT_PLUS = 0x34, TT_PLUSEQ = 0x35, TT_PLUSPLUS = 0x36, TT_MINUS = 0x37, TT_MINUSEQ = 0x38, TT_MINUSMINUS = 0x39,
    TT_STAR = 0x3a, TT_STAREQ = 0x3b, TT_SLASH = 0x3e, TT_SLASHEQ = 0x3f, TT_PERCENT = 0x40, TT_PERCENTEQ = 0x41,
    TT_LESS = 0x42, TT_LESSEQ = 0x43, TT_LSHIFT = 0x44, TT_LSHIFTEQ = 0x45,
    TT_GREAT = 0x46, TT_GREATEQ = 0x47, TT_RSHIFT = 0x48, TT_RSHIFTEQ = 0x49,
    TT_TILDE = 0x4a, TT_HAT = 0x4b, TT_HATEQ = 0x4c,
    TT_AMP = 0x4d, TT_AMPEQ = 0x4e, TT_AMPAMP = 0x4f, TT_PIPE = 0x50, TT_PIPEEQ = 0x51, TT_PIPEPIPE = 0x52
};

/**
 * @brief A seabow token that represent an analyzed part of code.
*/
class Token {
private:
    sbw_token_type tt; // The type of token.
    sbw_string text; // The text (or value) assigned to the token.
    sbw_ulong line, column; // The position of the token in its file.

public:
    /**
     * @brief Construct a new seabow token.
     * @param tt The type of token to create.
     * @param text The text / value to assigne to it.
     * @param line The line of the token in its file.
     * @param column The column of the token in its file.
    */
    inline Token(sbw_token_type tt, sbw_string text, sbw_ulong line, sbw_ulong column) {
        this->tt = tt;
        this->text = text;
        this->line = line;
        this->column = column;
    }

    /**
     * @brief Destroy a seabow token.
    */
    inline virtual ~Token() {}

    /**
     * @brief Get the type of the token.
     * @return The type of the token.
    */
    inline sbw_token_type Type() { return this->tt; }

    /**
     * @brief Get the text / value assigned to the token.
     * @return The text / value assigned to the token.
    */
    inline sbw_string Text() { return this->text; }

    /**
     * @brief Get the line position of the token in its file.
     * @return The line position of the token.
    */
    inline sbw_ulong Line() { return this->line; }

    /**
     * @brief Get the column position of the token in its file.
     * @return The column position of the token.
    */
    inline sbw_ulong Column() { return this->column; }

    /**
     * @brief <static> Get the precedence of an unary operator assigned to a type of token.
     * @param t The type of the token who represents an unary operator.
     * @return The operator precedence, or 0 if the type of token does not represents an unary operator.
    */
    inline static sbw_ubyte GetUnaryPrecedence(sbw_token_type t) {
        switch (t) {
            case TT_DOLLAR:
                return 18;

            case TT_PLUSPLUS: case TT_MINUSMINUS:
                return 16;

            case TT_NOT: case TT_TILDE: case TT_PLUS: case TT_MINUS:
                return 15;

            case TT_QUEST:
                return 2;

            default:
                return 0;
        }
    }

    /**
     * @brief <static> Get the precedence of a binary operator assigned to a type of token.
     * @param t The type of the token who represents a binary operator.
     * @return The operator precedence, or 0 if the type of token does not represents a binary operator.
     */
    inline static sbw_ubyte GetBinaryPrecedence(sbw_token_type t) {
        switch (t) {
            case TT_PLUSPLUS: case TT_MINUSMINUS:
                return 17;

            case TT_STAR: case TT_SLASH: case TT_PERCENT:
                return 13;

            case TT_PLUS: case TT_MINUS:
                return 12;

            case TT_LSHIFT: case TT_RSHIFT:
                return 11;

            case TT_IN: case TT_IS:
                return 10;

            case TT_LESS: case TT_LESSEQ: case TT_GREAT: case TT_GREATEQ:
                return 9;

            case TT_EE: case TT_NE:
                return 8;

            case TT_AMP:
                return 7;

            case TT_HAT:
                return 6;

            case TT_PIPE:
                return 5;

            case TT_AMPAMP:
                return 4;

            case TT_PIPEPIPE:
                return 3;

            case TT_EQ: case TT_PLUSEQ: case TT_MINUSEQ: case TT_STAREQ: case TT_SLASHEQ: case TT_PERCENTEQ:
            case TT_LSHIFTEQ: case TT_RSHIFTEQ: case TT_PIPEEQ: case TT_AMPEQ:
                return 1;

            default:
                return 0;
        }
    }

    /**
     * @brief <static> Get the type of seabow value who is associated to a certain keyword.
     * @param keyword The keyword that represents a type of seabow value.
     * @return The type of seabow value or SBW_VALUE_UNKNOWN if the given keyword is not associated to a type of seabow value.
    */
    inline static sbw_value_type KeywordType(std::string keyword) {
        if (keyword == "void") return SBW_NULL;
        else if (keyword == "byte") return SBW_BYTE;
        else if (keyword == "ubyte") return SBW_UBYTE;

        return SBW_VALUE_UNKNOWN;
    }
};

#endif // __SEABOW_TOKEN_HPP__