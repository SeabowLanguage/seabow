#include "nodes/node_unary.hpp"

NodeUnary::NodeUnary(sbw_ulong line, sbw_ulong column, Node *operand, sbw_token_type op_type)
    : Node(line, column)
{
    this->operand = operand;
    this->op_type = op_type;
}