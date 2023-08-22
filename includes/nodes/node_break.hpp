#ifndef __SEABOW_NODE_BREAK_HPP__
#define __SEABOW_NODE_BREAK_HPP__

#include "nodes/node_return.hpp"

class NodeBreak : public Node {
public:
    NodeBreak(sbw_ulong line, sbw_ulong column);
    inline virtual ~NodeBreak(sbw_none) {}

    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_BREAK; }
};

#endif // __SEABOW_NODE_BREAK_HPP__