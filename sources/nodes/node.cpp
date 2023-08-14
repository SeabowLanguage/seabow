#include "nodes/node.hpp"

Node::Node(sbw_ulong line, sbw_ulong column)
{
    this->line = line;
    this->column = column;
}