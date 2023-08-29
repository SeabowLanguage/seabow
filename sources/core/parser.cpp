#include "core/parser.hpp"

Parser::Parser(std::vector<Token*> tokens, std::unordered_map<sbw_string, NodeCompound*> *imports)
{
    this->tokens = tokens;
    this->pos = 0;
    if (imports != nullptr) {
        this->imports = imports;
    } else {
        this->imports = new std::unordered_map<sbw_string, NodeCompound*>();
    }
}

NodeCompound *Parser::Parse(sbw_none)
{
    std::vector<Node*> stats;

    Node *nd = this->ParseStatement(1);
    while (nd->Type() != SBW_NODE_UNKNOWN) {
        stats.push_back(nd);
        nd = this->ParseStatement(1);
    }

    return new NodeCompound(0, 0, stats);
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

NodeBad *Parser::Match(sbw_token_type t, sbw_string expected)
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
        node = this->ParseBinaryUnaryExpression(0);

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
    Token *start = this->Get();
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
    return new NodeCompound(start->Line(), start->Column(), stats);
}

Node *Parser::ParseConvertExpression(sbw_none)
{
    Token *current = this->Advance();
    Node *expression = this->ParseStatement(0);
    if (expression->Type() == SBW_NODE_BAD)
        return expression;
    
    NodeBad *err = this->Match(TT_RPAR, ")");
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

Node *Parser::ParseBinaryUnaryExpression(sbw_ubyte precedence)
{
    Node *left = nullptr;
    sbw_ubyte unary_precedence = Token::GetUnaryPrecedence(this->Get()->Type());
    if (unary_precedence != 0 && unary_precedence >= precedence) {
        if (unary_precedence == 2) {
            left = this->ParseQuestionOperator();
            if (left->Type() == SBW_NODE_BAD)
                return left;
        } else {
            Token *current = this->Advance();
            Node *operand = this->ParseBinaryUnaryExpression(unary_precedence);
            if (operand->Type() == SBW_NODE_BAD)
                return operand;
            left = new NodeUnary(current->Line(), current->Column(), operand, current->Type());
        }
    } else {
        left = this->ParsePrimaryExpression();
        if (left->Type() == SBW_NODE_BAD)
            return left;
    }

    while (true) {
        Token *current = this->Get();
        sbw_ubyte binary_precedence = Token::GetBinaryPrecedence(current->Type());
        if (binary_precedence == 0 || binary_precedence <= precedence)
            break;
        
        this->Advance();
        if (binary_precedence == 17)
            return new NodeBinary(current->Line(), current->Column(), left, nullptr, current->Type());

        Node *right = this->ParseBinaryUnaryExpression(binary_precedence);
        if (right->Type() == SBW_NODE_BAD)
            return right;
        
        left = new NodeBinary(current->Line(), current->Column(), left, right, current->Type());
    }

    return left;
}

Node *Parser::ParsePrimaryExpression(sbw_none)
{
    Token *current = this->Get();
    switch (current->Type()) {
        case TT_LPAR: {
            this->Advance();
            Node *expression = this->ParseStatement(0);
            if (expression->Type() == SBW_NODE_BAD)
                return expression;
            
            NodeBad *err = this->Match(TT_RPAR, ")");
            if (err) return err;

            return new NodeParenthesized(current->Line(), current->Column(), expression);
        }

        case TT_INTEGER: {
            return new NodeLiteral(current->Line(), current->Column(), nullptr);
        }

        case TT_LONG_INTEGER: {

        }

        case TT_DECIMAL: {

        }

        case TT_LONG_DECIMAL: {

        }

        case TT_CHAR: {

        }

        case TT_STRING: {

        }

        case TT_BOOLEAN: {

        }

        case TT_NULL: {

        }

        case TT_BAD: {

        }

        case TT_CONTROLLER: {

        }

        case TT_WORD: {

        }

        default: {
            this->Advance();
            return new NodeBad(new SBW_ValueError("SyntaxError", "Incorrect statement found", current->Line(), current->Column()));
        }
    }
}

Node *Parser::ParseQuestionOperator(sbw_none)
{
    return nullptr;
}

sbw_none Parser::ReUse(std::vector<Token*> tokens)
{
    this->tokens = tokens;
    this->pos = 0;
}