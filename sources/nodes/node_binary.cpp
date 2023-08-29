#include "nodes/node_binary.hpp"

NodeBinary::NodeBinary(sbw_ulong line, sbw_ulong column, Node *left, Node *right, sbw_token_type op_type)
    : Node(line, column)
{
    this->left = left;
    this->right = right;
    this->op_type = op_type;
}