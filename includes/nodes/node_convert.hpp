#ifndef __SEABOW_NODE_CONVERT_HPP__
#define __SEABOW_NODE_CONVERT_HPP__

#include "nodes/node_continue.hpp"

class NodeConvert : public Node {
private:
    Node *expression;

public:
    NodeConvert(sbw_ulong line, sbw_ulong column, Node *expression);
    inline virtual ~NodeConvert(sbw_none) {}

    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_CONVERT; }
};

#endif // __SEABOW_NODE_CONVERT_HPP__