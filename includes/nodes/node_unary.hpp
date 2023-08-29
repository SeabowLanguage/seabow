/**
 * @file node_unary.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents an unary calculation expression.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_UNARY_HPP__
#define __SEABOW_NODE_UNARY_HPP__

#include "nodes/node_convert.hpp"

/**
 * @brief A seabow node that represents an unary calculation expression.
*/
class NodeUnary: public Node {
private:
    Node *operand; // The operand of the unary calculation expression.
    sbw_token_type op_type; // The type of operator of the unary calculation expression.

public:
    /**
     * @brief Construct a new seabow nodeunary.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
     * @param operand The operand of the unary calculation expression.
     * @param op_type The type of operator of the unary calculation expression.
    */
    NodeUnary(sbw_ulong line, sbw_ulong column, Node *operand, sbw_token_type op_type);

    /**
     * @brief Destroy a seabow nodeunary.
    */
    inline virtual ~NodeUnary() {}

    /**
     * @brief Get the seabow node's type of a seabow nodeunary (i.e. SBW_NODE_UNARY).
     * @return SBW_NODE_UNARY.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_UNARY; }
    
    /**
     * @brief Get the operand of the unary calculation expression.
     * @return The operand of the unary calculation expression. 
    */
    inline Node *Operand(sbw_none) const { return this->operand; }
    
    /**
     * @brief Get the type of operator of the unary calculation expression.
     * @return The type of operator of the unary calculation expression.
    */
    inline sbw_token_type OpType(sbw_none) const { return this->op_type; }
};

#endif // __SEABOW_NODE_UNARY_HPP__