/**
 * @file lexer.cpp
 * @author LucaStarz
 * @brief Source code of the seabow's lexer.
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
*/

#include "core/lexer.hpp"

Lexer::Lexer(sbw_string code)
{
    this->code = code;
    this->pos = 0;
    this->line = this->column = 1;
}

Token *Lexer::Lex()
{
    this->SkipSpaces();
    char current = this->Get();
    if (current == '\0')
        return new Token(TT_EOF, "", this->line, this->column);
    
    if (current == '/') {
        char next = this->Get(1);
        if (next == '/' || next == '*') {
            this->SkipComments();
            return this->Lex();
        } else if (next == '=')
            return this->AdvanceWith("/=", 2, TT_SLASHEQ);
        return this->AdvanceWith("/", 1, TT_SLASH);
    }

    if (isalpha(current) || current == '_')
        return this->LexWord();

    switch (current) {
        case '\n': {
            sbw_ulong column = this->column;
            sbw_ulong line = this->line;
            this->Advance();
            this->column = 1;
            this->line++;
            return new Token(TT_NEW_LINE, "\n", line, column);
        }

        case '(': return this->AdvanceWith("(", 1, TT_LPAR);
        case ')': return this->AdvanceWith(")", 1, TT_RPAR);
        case '{': return this->AdvanceWith("{", 1, TT_LBRACE);
        case '}': return this->AdvanceWith("}", 1, TT_RBRACE);
        case '[': return this->AdvanceWith("[", 1, TT_LBRACKET);
        case ']': return this->AdvanceWith("]", 1, TT_RBRACKET);

        case ';': return this->AdvanceWith(";", 1, TT_SEMI);
        case ',': return this->AdvanceWith(",", 1, TT_COMMA);
        case ':': return this->AdvanceWith(":", 1, TT_COLON);
        case '.': return this->AdvanceWith(".", 1, TT_DOT);

        case '?': return this->AdvanceWith("?", 1, TT_QUEST);
        case '$': return this->AdvanceWith("$", 1, TT_DOLLAR);
        case '@': return this->AdvanceWith("@", 1, TT_AT);
        case '#': return this->AdvanceWith("#", 1, TT_HASH);

        case '+': {
            if (this->Get(1) == '+')
                return this->AdvanceWith("++", 2, TT_PLUSPLUS);
            else if (this->Get(1) == '=')
                return this->AdvanceWith("+=", 2, TT_PLUSEQ);
            return this->AdvanceWith("+", 1, TT_PLUS);
        }

        case '-': {
            if (this->Get(1) == '-')
                return this->AdvanceWith("--", 2, TT_MINUSMINUS);
            else if (this->Get(1) == '=')
                return this->AdvanceWith("-=", 2, TT_MINUSEQ);
            return this->AdvanceWith("-", 1, TT_MINUS);
        }

        case '*': {
            if (this->Get(1) == '*')
                return this->Get(2) == '=' ? this->AdvanceWith("**=", 3, TT_POWEREQ) : this->AdvanceWith("**", 2, TT_POWER);
            else if (this->Get(1) == '=')
                return this->AdvanceWith("*=", 2, TT_STAREQ);
            return this->AdvanceWith("*", 1, TT_STAR);
        }

        case '=': return this->Get(1) == '=' ? this->AdvanceWith("==", 2, TT_EE) : this->AdvanceWith("=", 1, TT_EQ);
        case '!': return this->Get(1) == '=' ? this->AdvanceWith("!=", 2, TT_NE) : this->AdvanceWith("!", 1, TT_NOT);

        case '<': {
            if (this->Get(1) == '<')
                return this->Get(2) == '=' ? this->AdvanceWith("<<=", 3, TT_LSHIFTEQ) : this->AdvanceWith("<<", 2, TT_LSHIFT);
            else if (this->Get(1) == '=')
                return this->AdvanceWith("<=", 2, TT_LESSEQ);
            return this->AdvanceWith("<", 1, TT_LESS);
        }

        case '>': {
            if (this->Get(1) == '>')
                return this->Get(2) == '=' ? this->AdvanceWith(">>=", 3, TT_RSHIFTEQ) : this->AdvanceWith(">>", 2, TT_RSHIFT);
            else if (this->Get(1) == '=')
                return this->AdvanceWith(">=", 2, TT_GREATEQ);
            return this->AdvanceWith(">", 1, TT_GREAT);
        }

        case '~': return this->AdvanceWith("~", 1, TT_TILDE);
        case '^': return this->Get(1) == '=' ? this->AdvanceWith("^=", 2, TT_HATEQ) : this->AdvanceWith("^", 1, TT_HAT);

        case '&': {
            if (this->Get(1) == '&')
                return this->AdvanceWith("&&", 2, TT_AMPAMP);
            else if (this->Get(1) == '=')
                return this->AdvanceWith("&=", 2, TT_AMPEQ);
            return this->AdvanceWith("&", 1, TT_AMP);
        }

        case '|': {
            if (this->Get(1) == '|')
                return this->AdvanceWith("||", 2, TT_PIPEPIPE);
            else if (this->Get(1) == '=')
                return this->AdvanceWith("|=", 2, TT_PIPEEQ);
            return this->AdvanceWith("|", 1, TT_PIPE);
        }
    }

    sbw_string bad_text = "Unexpected character '"; bad_text += current; bad_text += '\'';
    Token *bad = new Token(TT_BAD, bad_text, this->line, this->column);
    this->Advance();
    return bad;
}

char Lexer::Get(sbw_none)
{
    return this->pos < this->code.size() ? this->code.at(this->pos) : '\0';
}

char Lexer::Get(sbw_long offset)
{
    sbw_ulong index = this->pos + offset;
    return index < this->code.size() ? this->code.at(index) : '\0';   
}

sbw_none Lexer::Advance(sbw_none)
{
    this->pos++;
    this->column++;
}

Token *Lexer::AdvanceWith(sbw_string txt, sbw_ubyte size, sbw_token_type tt)
{
    sbw_ulong col = this->column;
    for (sbw_ubyte i=0; i<size; i++)
        this->Advance();
    
    return new Token(tt, txt, this->line, col);
}

sbw_none Lexer::SkipSpaces(sbw_none)
{
    char current = this->Get();
    while (isspace(current) && current != '\n') {
        this->Advance();
        current = this->Get();
    }
}

sbw_none Lexer::SkipComments(sbw_none)
{
    this->Advance();
    char current = this->Get();
    if (current == '*') {
        this->Advance();
        current = this->Get();
        while (current != '\0') {
            if (current == '\n') {
                this->Advance();
                this->column = 1;
                this->line++;
                current = this->Get();
                continue;
            }

            if (current == '*') {
                if (this->Get(1) == '/') {
                    this->Advance();
                    this->Advance();
                    return;
                }
            }

            this->Advance();
            current = this->Get();
        }
    }
    else {
        while (current != '\n' && current != '\0') {
            this->Advance();
            current = this->Get();
        }

        if (current != '\0') {
            this->Advance();
            this->column = 1;
            this->line++;
        }
    }
}

Token *Lexer::LexWord(sbw_none)
{
    sbw_string word;
    sbw_ulong column = this->column;
    sbw_char current = this->Get();
    while (isalpha(current) || current == '_') {
        word += current;
        this->Advance();
        current = this->Get();
    }

    if (word == "null")
        return new Token(TT_NULL, word, this->line, column);
    else if (word == "true" || word == "false")
        return new Token(TT_BOOLEAN, word, this->line, column);
    else if (word == "break" || word == "continue")
        return new Token(TT_CONTROLLER, word, this->line, column);
    else if (word == "is")
        return new Token(TT_IS, word, this->line, column);
    else if (word == "in")
        return new Token(TT_IN, word, this->line, column);
    return new Token(TT_WORD, word, this->line, column);
}