#include "nodes/node_literal.hpp"

NodeLiteral::NodeLiteral(sbw_ulong line, sbw_ulong column, SBW_Value *value)
    : Node(line, column)
{
    this->value = value;
}