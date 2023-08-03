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
    sbw_char current = this->Get();
    if (current == L'\0')
        return new Token(TT_EOF, L"", this->line, this->column);
    
    if (current == L'/') {
        sbw_char next = this->Get(1);
        if (next == L'/' || next == L'*')
            return this->SkipComments() ? new Token(TT_NEW_LINE, L"\n", this->line, this->column) : this->Lex();
        else if (next == L'=')
            return this->AdvanceWith(L"/=", 2, TT_SLASHEQ);
        return this->AdvanceWith(L"/", 1, TT_SLASH);
    }

    if (isalpha(current) || current == L'_')
        return this->LexWord();

    switch (current) {
        case L'\n': {
            sbw_ulong column = this->column;
            sbw_ulong line = this->line;
            this->Advance();
            this->column = 1;
            this->line++;
            return new Token(TT_NEW_LINE, L"\n", line, column);
        }

        case L'(': return this->AdvanceWith(L"(", 1, TT_LPAR);
        case L')': return this->AdvanceWith(L")", 1, TT_RPAR);
        case L'{': return this->AdvanceWith(L"{", 1, TT_LBRACE);
        case L'}': return this->AdvanceWith(L"}", 1, TT_RBRACE);
        case L'[': return this->AdvanceWith(L"[", 1, TT_LBRACKET);
        case L']': return this->AdvanceWith(L"]", 1, TT_RBRACKET);

        case L';': return this->AdvanceWith(L";", 1, TT_SEMI);
        case L',': return this->AdvanceWith(L",", 1, TT_COMMA);
        case L':': return this->AdvanceWith(L":", 1, TT_COLON);
        case L'.': return this->AdvanceWith(L".", 1, TT_DOT);

        case L'?': return this->AdvanceWith(L"?", 1, TT_QUEST);
        case L'$': return this->AdvanceWith(L"$", 1, TT_DOLLAR);
        case L'@': return this->AdvanceWith(L"@", 1, TT_AT);
        case L'#': return this->AdvanceWith(L"#", 1, TT_HASH);

        case L'+': {
            if (this->Get(1) == '+')
                return this->AdvanceWith(L"++", 2, TT_PLUSPLUS);
            else if (this->Get(1) == L'=')
                return this->AdvanceWith(L"+=", 2, TT_PLUSEQ);
            return this->AdvanceWith(L"+", 1, TT_PLUS);
        }

        case L'-': {
            if (this->Get(1) == L'-')
                return this->AdvanceWith(L"--", 2, TT_MINUSMINUS);
            else if (this->Get(1) == L'=')
                return this->AdvanceWith(L"-=", 2, TT_MINUSEQ);
            return this->AdvanceWith(L"-", 1, TT_MINUS);
        }

        case L'*': {
            if (this->Get(1) == L'*')
                return this->Get(2) == L'=' ? this->AdvanceWith(L"**=", 3, TT_POWEREQ) : this->AdvanceWith(L"**", 2, TT_POWER);
            else if (this->Get(1) == L'=')
                return this->AdvanceWith(L"*=", 2, TT_STAREQ);
            return this->AdvanceWith(L"*", 1, TT_STAR);
        }

        case L'=': return this->Get(1) == L'=' ? this->AdvanceWith(L"==", 2, TT_EE) : this->AdvanceWith(L"=", 1, TT_EQ);
        case L'!': return this->Get(1) == L'=' ? this->AdvanceWith(L"!=", 2, TT_NE) : this->AdvanceWith(L"!", 1, TT_NOT);

        case L'<': {
            if (this->Get(1) == L'<')
                return this->Get(2) == L'=' ? this->AdvanceWith(L"<<=", 3, TT_LSHIFTEQ) : this->AdvanceWith(L"<<", 2, TT_LSHIFT);
            else if (this->Get(1) == L'=')
                return this->AdvanceWith(L"<=", 2, TT_LESSEQ);
            return this->AdvanceWith(L"<", 1, TT_LESS);
        }

        case L'>': {
            if (this->Get(1) == L'>')
                return this->Get(2) == L'=' ? this->AdvanceWith(L">>=", 3, TT_RSHIFTEQ) : this->AdvanceWith(L">>", 2, TT_RSHIFT);
            else if (this->Get(1) == L'=')
                return this->AdvanceWith(L">=", 2, TT_GREATEQ);
            return this->AdvanceWith(L">", 1, TT_GREAT);
        }

        case L'~': return this->AdvanceWith(L"~", 1, TT_TILDE);
        case L'^': return this->Get(1) == L'=' ? this->AdvanceWith(L"^=", 2, TT_HATEQ) : this->AdvanceWith(L"^", 1, TT_HAT);

        case L'&': {
            if (this->Get(1) == L'&')
                return this->AdvanceWith(L"&&", 2, TT_AMPAMP);
            else if (this->Get(1) == L'=')
                return this->AdvanceWith(L"&=", 2, TT_AMPEQ);
            return this->AdvanceWith(L"&", 1, TT_AMP);
        }

        case L'|': {
            if (this->Get(1) == L'|')
                return this->AdvanceWith(L"||", 2, TT_PIPEPIPE);
            else if (this->Get(1) == L'=')
                return this->AdvanceWith(L"|=", 2, TT_PIPEEQ);
            return this->AdvanceWith(L"|", 1, TT_PIPE);
        }
    }

    sbw_string bad_text = L"Unexpected character '"; bad_text += current; bad_text += L'\'';
    Token *bad = new Token(TT_BAD, bad_text, this->line, this->column);
    this->Advance();
    return bad;
}

sbw_char Lexer::Get(sbw_none)
{
    return this->pos < this->code.size() ? this->code.at(this->pos) : L'\0';
}

sbw_char Lexer::Get(sbw_long offset)
{
    sbw_ulong index = this->pos + offset;
    return index < this->code.size() ? this->code.at(index) : L'\0';   
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
    sbw_char current = this->Get();
    while (isspace(current) && current != L'\n') {
        this->Advance();
        current = this->Get();
    }
}

sbw_bool Lexer::SkipComments(sbw_none)
{
    this->Advance();
    sbw_char current = this->Get();
    if (current == L'*') {
        bool nl = false;
        this->Advance();
        current = this->Get();
        while (current != L'\0') {
            if (current == L'\n') {
                this->Advance();
                this->column = 1;
                this->line++;
                current = this->Get();
                nl = true;
                continue;
            }

            if (current == L'*') {
                if (this->Get(1) == L'/') {
                    this->Advance();
                    this->Advance();
                    return nl;
                }
            }

            this->Advance();
            current = this->Get();
        }

        return nl;
    }
    else {
        while (current != L'\n' && current != L'\0') {
            this->Advance();
            current = this->Get();
        }

        this->Advance();
        this->column = 1;
        this->line++;
        return true;
    }
}

Token *Lexer::LexWord(sbw_none)
{
    sbw_string word;
    sbw_ulong column = this->column;
    sbw_char current = this->Get();
    while (isalpha(current) || current == L'_') {
        word += current;
        this->Advance();
        current = this->Get();
    }

    if (word == L"null")
        return new Token(TT_NULL, word, this->line, column);
    else if (word == L"true" || word == L"false")
        return new Token(TT_BOOLEAN, word, this->line, column);
    else if (word == L"break" || word == L"continue")
        return new Token(TT_CONTROLLER, word, this->line, column);
    else if (word == L"is")
        return new Token(TT_IS, word, this->line, column);
    else if (word == L"in")
        return new Token(TT_IN, word, this->line, column);
    return new Token(TT_WORD, word, this->line, column);
}

sbw_char Lexer::SpecialChar(sbw_none)
{

}

Token *Lexer::LexCharacter(sbw_none)
{
    sbw_ulong column = this->column;
    return new Token(TT_CHAR, L"aq", this->line, column);
}