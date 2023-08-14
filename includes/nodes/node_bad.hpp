#ifndef __SEABOW_NODE_BAD_HPP__
#define __SEABOW_NODE_BAD_HPP__

#include "node.hpp"

class NodeBad: Node {
private:
    SBW_ValueError *error;

public:
    NodeBad(sbw_ulong line, sbw_ulong column, SBW_ValueError *err);
    inline virtual ~NodeBad(sbw_none) {}

    inline SBW_ValueError *Error(sbw_none) { return this->error; }
};

#endif // __SEABOW_NODE_BAD_HPP__