#ifndef __SEABOW_PARSER_HPP__
#define __SEABOW_PARSER_HPP__

#include "nodes/node_compound.hpp"

class Parser {
private:
    std::vector<Token*> tokens;
    sbw_ulong pos;

    Token *Advance(sbw_none);
    Node *ParseStatement(sbw_none);
public:
    Parser(sbw_string code);
    inline virtual ~Parser() {}

    NodeCompound *Parse(sbw_none);
};

#endif // __SEABOW_PARSER_HPP__