/**
 * @file node_binary.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a binary calculation expression.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_BINARY_HPP__
#define __SEABOW_NODE_BINARY_HPP__

#include "nodes/node_unary.hpp"

/**
 * @brief A seabow node that represents a binary calculation expression.
*/
class NodeBinary: public Node {
private:
    Node *left, *right; // The operands of the binary calculation expression.
    sbw_token_type op_type; // The type of operator of the binary calculation expression.

public:
    /**
     * @brief Construct a new seabow nodebinary.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
     * @param left The left operand of the binary calculation expression.
     * @param right The right operand of the binary calculation expression.
     * @param op_type The type of operator of the binary calculation expression.
    */
    NodeBinary(sbw_ulong line, sbw_ulong column, Node *left, Node *right, sbw_token_type op_type);
    
    /**
     * @brief Destroy a seabow nodebinary.
    */
    inline virtual ~NodeBinary() {}

    /**
     * @brief Get the seabow node's type of a seabow nodebinary (i.e. SBW_NODE_BINARY).
     * @return SBW_NODE_BINARY.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_BINARY; }
    
    /**
     * @brief Get the left operand of the binary calculation expression.
     * @return The left operand.
    */
    inline Node *Left(sbw_none) const { return this->left; }
    
    /**
     * @brief Get the right operand of the binary calculation expression.
     * @return The right operand.
    */
    inline Node *Right(sbw_none) const { return this->right; }
    
    /**
     * @brief Get the type of operator of the binary calculation expression.
     * @return The type of operator.
    */
    inline sbw_token_type OpType(sbw_none) const { return this->op_type; }
};

#endif // __SEABOW_NODE_BINARY_HPP__