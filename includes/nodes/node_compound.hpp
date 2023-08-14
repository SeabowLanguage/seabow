#ifndef __SEABOW_NODE_COMPOUND_HPP__
#define __SEABOW_NODE_COMPOUND_HPP__

#include "node_bad.hpp"

class NodeCompound: Node {
private:
    std::vector<Node*> statements;

public:
    NodeCompound(std::vector<Node*> stats);
    inline virtual ~NodeCompound(sbw_none) {}

    inline std::vector<Node*> Statements(sbw_none) { return this->statements; }
};

#endif // __SEABOW_NODE_COMPOUND_HPP__