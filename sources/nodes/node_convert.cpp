#include "nodes/node_convert.hpp"

NodeConvert::NodeConvert(sbw_ulong line, sbw_ulong column, Node *expression)
    : Node(line, column)
{
    this->expression = expression;
}