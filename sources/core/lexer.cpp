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
    this->wait_for_chars = 0;
}

Token *Lexer::Lex()
{
    this->SkipSpaces();
    char current = this->Get();
    if (current == '\0')
        return new Token(TT_EOF, "<EOF>", this->line, this->column);
    
    if (current == '/') {
        char next = this->Get(1);
        if (next == '/' || next == '*')
            return this->SkipComments() ? new Token(TT_NEW_LINE, "<NL>", this->line, this->column) : this->Lex();
        else if (next == '=')
            return this->AdvanceWith("/=", 2, TT_SLASHEQ);
        return this->AdvanceWith("/", 1, TT_SLASH);
    }

    if (isalpha(current) || current == '_')
        return this->LexWord();
    
    if (current >= '0' && current <= '9')
        return this->LexNumber();
    
    if (current == '"')
        return this->LexString();
    
    if (current == '\'')
        return this->LexCharacter();

    switch (current) {
        case '\n': {
            sbw_ulong column = this->column;
            sbw_ulong line = this->line;
            this->Advance();
            this->column = 1;
            this->line++;
            return new Token(TT_NEW_LINE, "<NL>", line, column);
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
    
    this->wait_for_chars--;
    if (this->wait_for_chars <= 0) {
        this->column++;
        char current = this->Get();
        if (((unsigned char)current & 0x80) == 0)
            this->wait_for_chars = 1;
        else if (((unsigned char)current >> 5) == 0x06)
            this->wait_for_chars = 2;
        else if (((unsigned char)current >> 4) == 0x0E)
            this->wait_for_chars = 3;
        else if (((unsigned char)current >> 3) == 0x1E)
            this->wait_for_chars = 4;
    }
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

sbw_none Lexer::SpecialSkipSpaces(sbw_none)
{
    char current = this->Get();
    while (isspace(current) && current != ' ') {
        this->Advance();
        if (current == '\n') {
            this->line++;
            this->column = 1;
        }
        current = this->Get();
    }
}

sbw_bool Lexer::SkipComments(sbw_none)
{
    this->Advance();
    char current = this->Get();
    if (current == '*') {
        bool nl = false;
        this->Advance();
        current = this->Get();
        while (current != '\0') {
            if (current == '\n') {
                this->Advance();
                this->column = 1;
                this->line++;
                current = this->Get();
                nl = true;
                continue;
            }

            if (current == '*') {
                if (this->Get(1) == '/') {
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
        while (current != '\n' && current != '\0') {
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
    char current = this->Get();
    while (isalpha(current) || current == '_' || (current >= '0' && current <= '9')) {
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
    else if (word == "as")
        return new Token(TT_AS, word, this->line, column);
    return new Token(TT_WORD, word, this->line, column);
}

Token *Lexer::LexNumber(sbw_none)
{
    sbw_string number;
    sbw_ulong column = this->column;
    sbw_ulong dots = 0, exps = 0;
    char mode = 'i'; // i: integer, o: octal, x: hexadecimal, b: binary, f: floating, I: (u)int128, F: ldouble

    char first = this->Get();
    this->Advance();
    char current = this->Get();
    if (first == '0') {
        if (current == 'o') { mode = 'o'; this->Advance(); current = this->Get(); }
        else if (current == 'x') { mode = 'x'; this->Advance(); current = this->Get(); }
        else if (current == 'b') { mode = 'b'; this->Advance(); current = this->Get(); }
        else number += first;
    } else
        number += first;

    while ((current >= '0' && current <= '9') || current == '_' || current == '.' || current == 'e' || current == 'E') {
        if (current == 'e' || current == 'E') {
            if (mode != 'i' && mode != 'f') {
                this->Advance();
                if (this->Get() == '-' || this->Get() == '+') this->Advance();
                while ((current >= '0' && current <= '9') || current == '_' || current == '.' || current == 'e' || current == 'E') {
                    this->Advance();
                    current = this->Get();
                } return new Token(TT_BAD, "Hexadecimal, octal or binary number can not be exponential number", this->line, column);
            }
                
            mode = 'f';
            exps++;
            number += 'e';
            this->Advance();
            current = this->Get();
            if (current != '-' && current != '+') {
                while ((current >= '0' && current <= '9') || current == '_' || current == '.' || current == 'e' || current == 'E') {
                    this->Advance();
                    current = this->Get();
                } return new Token(TT_BAD, "Exponential number must have '+' or '-' after 'e'", this->line, column);
            }
            
            number += current;
            this->Advance();
            current = this->Get();
            continue;
        } else if (current == '.') {
            if (mode != 'i' && mode != 'f') {
                this->Advance();
                while ((current >= '0' && current <= '9') || current == '_' || current == '.' || current == 'e' || current == 'E') {
                    if ((current == 'e' || current == 'E') && (current == '-' || current == '+'))
                        this->Advance();
                    this->Advance();
                    current = this->Get();
                } return new Token(TT_BAD, "Hexadecimal, octal or binary number can not be decimal number", this->line, column);
            }

            mode = 'f';
            dots++;
        } else if (current == '_') {
            this->Advance();
            current = this->Get();
            continue;
        }

        number += current;
        this->Advance();
        current = this->Get();
    }

    if (this->Get() == 'L') {
        this->Advance();
        if (mode == 'f')
            return new Token(TT_BAD, "Decimal number can not have modifier 'L'", this->line, column);
        mode = 'I';
    }
    else if (this->Get() == 'D') {
        this->Advance();
        mode = 'F';
    }
    
    if (dots > 1)
        return new Token(TT_BAD, "Decimal number can not have more than one '.'", this->line, column);
    if (exps > 1)
        return new Token(TT_BAD, "Exponential number can not have more than one 'e'", this->line, column);

    char end_number = number.back();
    if (end_number == '-' || end_number == '+')
        return new Token(TT_BAD, "Exponential number must have at least 1 digit after 'e+' or 'e-'", this->line, column);
    else if (end_number == '.')
        return new Token(TT_BAD, "Decimal number must have at least 1 digit after '.'", this->line, column);

    switch (mode) {
        case 'o': return new Token(TT_OCTAL, number, this->line, column);
        case 'x': return new Token(TT_HEXADECIMAL, number, this->line, column);
        case 'b': return new Token(TT_BINARY, number, this->line, column);
        case 'f': return new Token(TT_DECIMAL, number, this->line, column);
        case 'I': return new Token(TT_LONG_INTEGER, number, this->line, column);
        case 'F': return new Token(TT_LONG_DECIMAL, number, this->line, column);
        case 'i': default: return new Token(TT_INTEGER, number, this->line, column);
    }
}

sbw_string Lexer::SpecialChar(sbw_none)
{
    sbw_string c;

    this->Advance();
    switch (this->Get()) {
        case '0': c += "\0"; break;
        case 'n': c += "\n"; break;
        case 't': c += "\t"; break;
        case 'r': c += "\r"; break;
        case 'f': c += "\f"; break;
        case 'v': c += "\v"; break;
        case 'b': c += "\b"; break;
        case 'a': c += "\a"; break;

        case 'u': case 'x': { // Hexadecimal char (max: 8 hex)
            this->Advance();
            sbw_string hex;
            char current = this->Get();
            if (!(current >= '0' && current <= '9') && !(current >= 'a' && current <= 'f') && !(current >= 'A' && current <= 'F'))
                return sbw_string("x");
            
            hex += current;
            this->Advance();
            current = this->Get();
            for (sbw_ubyte i=0; i<7; i++) {
                if (!(current >= '0' && current <= '9') && !(current >= 'a' && current <= 'f') && !(current >= 'A' && current <= 'F'))
                    break;
                
                hex += current;
                this->Advance();
                current = this->Get();
            }

            if (hex.size() % 2 != 0) hex.insert(0, 1, '0');
            for (sbw_ubyte i=0; i<hex.size()/2; i++)
                c += (char)(std::stoi(hex.substr(2*i, 2), nullptr, 16));
            return c;
        } break;

        case 'o': { // Octal char (max 11 oct with first max 3)
            this->Advance();
            sbw_string oct;
            char current = this->Get();
            if (current > '7' || current < '0')
                return sbw_string("o");
            
            oct += current;
            sbw_ubyte oct_limit = current > '3' ? 9 : 10;
            this->Advance();
            current = this->Get();
            for (sbw_ubyte i=0; i<oct_limit; i++) {
                if (current > '7' || current < '0')
                    break;
                
                oct += current;
                this->Advance();
                current = this->Get();
            }

            if (oct.size() % 4 != 0) oct.insert(0, 4 - (oct.size() % 4), '0');
            for (sbw_ubyte i=0; i<oct.size()/4; i++)
                c += (char)(std::stoi(oct.substr(4*i, 4), nullptr, 8));
            return c;
        } break;

        case 'B': { // Binary char (max 32 bin)
            this->Advance();
            sbw_string bin;
            char current = this->Get();
            if (current != '0' && current != '1')
                return sbw_string("B");
            
            bin += current;
            this->Advance();
            current = this->Get();
            for (sbw_ubyte i=0; i<31; i++) {
                if (current != '0' && current != '1')
                    break;
                
                bin += current;
                this->Advance();
                current = this->Get();
            }

            if (bin.size() % 8 != 0) bin.insert(0, 8 - (bin.size() % 8), '0');
            for (sbw_ubyte i=0; i<bin.size()/8; i++)
                c += (char)(std::stoi(bin.substr(8*i, 8), nullptr, 2));
            return c;
        } break;

        default: c += this->Get(); break;
    }

    this->Advance();
    return c;
}

Token *Lexer::LexCharacter(sbw_none)
{
    sbw_ulong line = this->line;
    sbw_ulong column = this->column;
    this->Advance();
    this->SpecialSkipSpaces();
    sbw_string c;
    char current = this->Get();
    if (current == '\\')
        c = this->SpecialChar();
    else {
        sbw_ubyte nb_char = 1;
        if (((unsigned char)current & 0x80) == 0)
            nb_char = 1;
        else if (((unsigned char)current >> 5) == 0x06)
            nb_char = 2;
        else if (((unsigned char)current >> 4) == 0x0E)
            nb_char = 3;
        else if (((unsigned char)current >> 3) == 0x1E)
            nb_char = 4;
        
        for (; nb_char>0; nb_char--) {
            c += current;
            this->Advance();
            current = this->Get();
        }
    }

    this->SpecialSkipSpaces();
    if (this->Get() != '\'') {
        while (this->Get() != '\'' && this->Get() != '\0') {
            if (this->Get() == '\n') {
                this->Advance();
                this->column = 1;
                this->line++;
            } else
                this->Advance();
        }
        return new Token(TT_BAD, "Unterminated character", line, column);
    }

    this->Advance();
    return new Token(TT_CHAR, c, line, column);
}

Token *Lexer::LexString(sbw_none)
{
    sbw_ulong line = this->line;
    sbw_ulong column = this->column;
    this->Advance();
    this->SpecialSkipSpaces();
    sbw_string s;
    char current = this->Get();
    while (current != '"' && current != '\0') {
        if (current == '\\')
            s += this->SpecialChar();
        else {
            this->Advance();
            s += current;
        }

        this->SpecialSkipSpaces();
        current = this->Get();
    }
    
    this->SpecialSkipSpaces();
    if (this->Get() != '"') {
        while (this->Get() != '"' && this->Get() != '\0') {
            if (this->Get() == '\n') {
                this->Advance();
                this->column = 1;
                this->line++;
            } else
                this->Advance();
        }
        return new Token(TT_BAD, "Unterminated string", line, column);
    }

    this->Advance();
    return new Token(TT_STRING, s, line, column);
}