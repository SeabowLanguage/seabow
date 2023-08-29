#include "nodes/node_parenthesized.hpp"

NodeParenthesized::NodeParenthesized(sbw_ulong line, sbw_ulong column, Node *expression)
    : Node(line, column)
{
    this->expression = expression;
}