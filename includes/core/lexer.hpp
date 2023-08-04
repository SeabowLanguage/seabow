/**
 * @file lexer.hpp
 * @author LucaStarz
 * @brief Description of seabow's lexer.
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_LEXER_HPP__
#define __SEABOW_LEXER_HPP__

#include "token.hpp"

class Lexer {
private:
    sbw_string code;
    sbw_ulong pos, line, column;
    sbw_byte wait_for_chars;

    char Get(sbw_none); char Get(sbw_long offset);
    sbw_none Advance(sbw_none);
    Token *AdvanceWith(sbw_string txt, sbw_ubyte size, sbw_token_type tt);
    sbw_none SkipSpaces(sbw_none); sbw_none SpecialSkipSpaces(sbw_none);
    sbw_bool SkipComments(sbw_none);

    Token *LexWord(sbw_none);
    Token *LexNumber(sbw_none);
    
    sbw_string SpecialChar(sbw_none);
    Token *LexCharacter(sbw_none);
    Token *LexString(sbw_none);
public:
    Lexer(sbw_string code);
    inline virtual ~Lexer() {}

    Token *Lex();
};

#endif // __SEABOW_LEXER_HPP__
