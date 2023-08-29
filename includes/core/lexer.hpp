/**
 * @file lexer.hpp
 * @author LucaStarz
 * @brief Description of seabow's lexer.
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_LEXER_HPP__
#define __SEABOW_LEXER_HPP__

#include "token.hpp"

/**
 * @brief Seabow lexer for seabow source code (.sbw files) convert source code into seabow tokens.
*/
class Lexer {
private:
    sbw_string code; // Seabow source code.
    sbw_ulong pos, line, column; // Positions in seabow source code.
    sbw_byte wait_for_chars; // Used to wait while reading UTF-8 characters.

    /**
     * @brief Get the character in source code at the current position.
     * @return The current character or '\0' if the end of file is reached.
    */
    char Get(sbw_none); 

    /**
     * @brief Get the character in source code at the current position + an offset.
     * @param offset The offset to apply to the current position.
     * @return The character at specified position or '\0' if the end of file is reached.
    */
    char Get(sbw_long offset);
    
    /**
     * @brief Advance current position and line / column counters. 
    */
    sbw_none Advance(sbw_none);

    /**
     * @brief Advance current position and line / column counters then return a new seabow token.
     * @param txt The text of the seabow token.
     * @param size Specify how much advance the position.
     * @param tt The type of seabow token to create.
     * @return The new generated seabow token.
    */
    Token *AdvanceWith(sbw_string txt, sbw_ubyte size, sbw_token_type tt);
    
    /**
     * @brief Skip all spaces characters in seabow source code.
    */
    sbw_none SkipSpaces(sbw_none); 
    
    /**
     * @brief Skip all spaces characters in seabow source code except ' '.
    */
    sbw_none SpecialSkipSpaces(sbw_none);

    /**
     * @brief Skip all seabow comments in seabow source code.
    */
    sbw_bool SkipComments(sbw_none);

    /**
     * @brief Read a seabow word and translate it into a seabow token.
     * @return The seabow token that represents a seabow word.
    */
    Token *LexWord(sbw_none);

    /**
     * @brief Read a seabow number and translate it into a seabow token.
     * @return The seabow token that represents a seabow number.
    */
    Token *LexNumber(sbw_none);
    
    /**
     * @brief Read a special character from normal characters (i.e. starts with '\').
     * @return The special character.
    */
    sbw_string SpecialChar(sbw_none);

    /**
     * @brief Read a seabow character declaration and translate it into a seabow token.
     * @return The seabow token that represents a seabow character.
    */
    Token *LexCharacter(sbw_none);

    /**
     * @brief Read a seabow string declaration and translate it into a seabow token.
     * @return The seabow token that represents a seabow string.
    */
    Token *LexString(sbw_none);
public:
    /**
     * @brief Construct a new seabow lexer to analyze seabow source code.
     * @param code The seabow source code to analyze.
    */
    Lexer(sbw_string code);

    /**
     * @brief Destroy a seabow lexer.
    */
    inline virtual ~Lexer() {}

    /**
     * @brief Read a part of the seabow source code and translate it into a seabow token.
     * @return The seabow token that represents a part of the seabow source code.
    */
    Token *Lex();

    /**
     * @brief Re-use the lexer with another seabow source code.
     * @param code The seabow source code to analyze.
     */
    sbw_none ReUse(sbw_string code);
};

#endif // __SEABOW_LEXER_HPP__
