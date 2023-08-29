#include "nodes/node_compound.hpp"

NodeCompound::NodeCompound(sbw_ulong line, sbw_ulong column, std::vector<Node*> stats)
    : Node(line, column)
{
    this->statements = stats;
}