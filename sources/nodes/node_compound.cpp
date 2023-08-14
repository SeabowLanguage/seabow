#include "nodes/node_compound.hpp"

NodeCompound::NodeCompound(std::vector<Node*> stats)
    : Node(0, 0)
{
    this->statements = stats;
}