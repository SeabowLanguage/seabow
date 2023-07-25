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

    switch (current) {
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
    while (isspace(current)) {
        if (current == '\n') {
            this->Advance();
            this->column = 1;
            this->line++;
        } else
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

        this->Advance();
        this->column = 1;
        this->line++;
    }
}