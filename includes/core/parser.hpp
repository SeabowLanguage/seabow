#ifndef __SEABOW_PARSER_HPP__
#define __SEABOW_PARSER_HPP__

#include "nodes/node_compound.hpp"

class Parser {
private:
    std::vector<Token*> tokens;
    sbw_ulong pos;

    sbw_none SkipNewLines(sbw_none);
    Token *Advance(sbw_none);
    Token *Get(sbw_none); Token *Get(sbw_long offset);
    Node *Match(sbw_token_type t, sbw_string expected);

    Node *ParseStatement(sbw_ubyte is_stat);
    Node *ParseCompound(sbw_ubyte is_stat);
public:
    Parser(sbw_string code);
    inline virtual ~Parser() {}

    NodeCompound *Parse(sbw_none);
};

#endif // __SEABOW_PARSER_HPP__