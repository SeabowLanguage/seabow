#ifndef __SEABOW_NODE_COMPOUND_HPP__
#define __SEABOW_NODE_COMPOUND_HPP__

#include "node_bad.hpp"

class NodeCompound: public Node {
private:
    std::vector<Node*> statements;

public:
    NodeCompound(std::vector<Node*> stats);
    inline virtual ~NodeCompound(sbw_none) {}

    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_COMPOUND; }
    inline std::vector<Node*> Statements(sbw_none) { return this->statements; }
};

#endif // __SEABOW_NODE_COMPOUND_HPP__