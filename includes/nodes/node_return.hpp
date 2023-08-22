#ifndef __SEABOW_NODE_RETURN_HPP__
#define __SEABOW_NODE_RETURN_HPP__

#include "nodes/node_compound.hpp"

class NodeReturn : public Node {
private:
    Node *expression;

public:
    NodeReturn(sbw_ulong line, sbw_ulong column, Node *expression);
    inline virtual ~NodeReturn(sbw_none) {}

    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_RETURN; }
    inline Node *Expression(sbw_none) const { return this->expression; }
};

#endif // __SEABOW_NODE_RETURN_HPP__