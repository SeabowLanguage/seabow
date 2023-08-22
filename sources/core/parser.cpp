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
    else if (current->Type() == TT_WORD) {
        sbw_value_type vt = Token::KeywordType(current->Text());
        if (vt != SBW_VALUE_UNKNOWN) {
            if (this->Get(1)->Type() != TT_LPAR)
                node = this->ParseDeclaration(is_stat);
            else
                node = this->ParseConvertExpression();
        }

        // Other keywords
    }

    if (!node)
        node = this->ParseController(is_stat);
    
    if (!node)
        {} // Binary / Unary expression

    if (node->Type() == SBW_NODE_BAD) return node;
    if (!node) {
        current = this->Get();
        return new NodeBad(new SBW_ValueError("SyntaxError", "Invalid statement", current->Line(), current->Column()));
    }

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

Node *Parser::ParseConvertExpression(sbw_none)
{
    Token *current = this->Advance();
    Node *expression = this->ParseStatement(0);
    if (expression->Type() == SBW_NODE_BAD)
        return expression;
    
    Node *err = this->Match(TT_RPAR, ")");
    if (err) return err;

    return new NodeConvert(current->Line(), current->Column(), expression);
}

Node *Parser::ParseDeclaration(sbw_ubyte is_stat)
{
    return nullptr;
}

Node *Parser::ParseController(sbw_ubyte ctrl)
{
    Token *current = this->Get();
    if (ctrl % 10 >= 2) {
        if (current->Text() == "break")
            return new NodeBreak(current->Line(), this->Advance()->Column());
        else if (current->Text() == "continue")
            return new NodeContinue(current->Line(), this->Advance()->Column());
    } else if (ctrl % 100 >= 10 && current->Text() == "return") {
        current = this->Advance();
        Node *expression = this->ParseStatement(0);
        if (expression->Type() == SBW_NODE_BAD)
            return expression;
        return new NodeReturn(current->Line(), current->Column(), expression);
    }

    return nullptr;
}