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

    Node *nd = this->ParseStatement();
    while (nd->Type() != SBW_NODE_UNKNOWN) {
        stats.push_back(nd);
        nd = this->ParseStatement();
    }

    return new NodeCompound(stats);
}

Token *Parser::Advance(sbw_none)
{
    Token *ret = this->tokens.at(this->pos);
    this->pos++;
    return ret;
}

Node *Parser::ParseStatement(sbw_none)
{
    Token *tok = this->Advance();
    switch (tok->Type()) {

        case TT_EOF: return new Node(tok->Line(), tok->Column());
    }
}