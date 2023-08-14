#include "core/parser.hpp"

Parser::Parser(sbw_string code)
{
    Lexer *lex = new Lexer(code);
    Token *tok = lex->Lex();
    while (tok->Type() != TT_EOF) {
        this->tokens.push_back(tok);
        tok = lex->Lex();
    }
    this->tokens.push_back(tok);
    delete lex;
    this->pos = 0;
}

NodeCompound *Parser::Parse(sbw_none)
{
    std::vector<Node*> stats;

    Node *nd = this->ParseStatement(1);
    while (nd->Type() != SBW_NODE_UNKNOWN) {
        stats.push_back(nd);
        nd = this->ParseStatement(1);
    }

    return new NodeCompound(stats);
}

sbw_none Parser::SkipNewLines(sbw_none)
{
    sbw_token_type t = this->Get()->Type();
    while (t == TT_NEW_LINE) {
        this->pos++;
        t = this->Get()->Type();
    }
}

Token *Parser::Advance(sbw_none)
{
    Token *ret = this->tokens.at(this->pos);
    this->pos++;
    this->SkipNewLines();
    return ret;
}

Token *Parser::Get(sbw_none)
{
    return this->pos >= this->tokens.size() ? this->tokens.back() : this->tokens.at(this->pos);
}

Token *Parser::Get(sbw_long offset)
{
    sbw_ulong p = this->pos + offset;
    return p >= this->tokens.size() ? this->tokens.back() : this->tokens.at(p);
}

Node *Parser::Match(sbw_token_type t, sbw_string expected)
{
    Token *current = this->Advance();
    if (current->Type() != t)
        return new NodeBad(new SBW_ValueError("SyntaxError", "Expected '" + expected + "' but was '" + current->Text() + "'", current->Line(), current->Column()));
    
    return nullptr;
}

Node *Parser::ParseStatement(sbw_ubyte is_stat)
{
    this->SkipNewLines();
    Token *current = this->Get();
    Node *node = nullptr;
    if (current->Type() == TT_LBRACE)
        return this->ParseCompound(is_stat);

    if (is_stat % 2 == 1) {
        Token *act = this->Get();
        if (act->Type() != TT_NEW_LINE && act->Type() != TT_SEMI && act->Type() != TT_EOF)
            return new NodeBad(new SBW_ValueError("SyntaxError", "Need new line or semi-colon between two statements", act->Line(), act->Column()));
        this->Advance();
    }

    return node;
}

Node *Parser::ParseCompound(sbw_ubyte is_stat)
{
    this->Advance();
    std::vector<Node*> stats;
    Token *first = this->Get();
    sbw_ubyte is_statement = is_stat % 2 == 1 ? is_stat : is_stat + 1;

    Token *current = this->Get();
    while (current->Type() != TT_RBRACE && current->Type() != TT_EOF) {
        Node *stat = this->ParseStatement(is_statement);
        if (stat->Type() == SBW_NODE_BAD)
            return stat;
        stats.push_back(stat);
        current = this->Get();
    }

    if (current->Type() != TT_RBRACE)
        return this->Match(TT_RBRACE, "}");

    this->Advance();
    return new NodeCompound(stats);
}