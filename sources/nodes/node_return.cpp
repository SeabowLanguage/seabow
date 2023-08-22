#include "nodes/node_return.hpp"

NodeReturn::NodeReturn(sbw_ulong line, sbw_ulong column, Node *expression)
    : Node(line, column)
{
    this->expression = expression;
}