#ifndef __SEABOW_NODE_BAD_HPP__
#define __SEABOW_NODE_BAD_HPP__

#include "node.hpp"

class NodeBad: public Node {
private:
    SBW_ValueError *error;

public:
    NodeBad(SBW_ValueError *err);
    inline virtual ~NodeBad(sbw_none) {}

    inline SBW_ValueError *Error(sbw_none) { return this->error; }
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_BAD; }
};

#endif // __SEABOW_NODE_BAD_HPP__