/**
 * @file node_parenthesized.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a parenthesized seabow expression.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_PARENTHESIZED_HPP__
#define __SEABOW_NODE_PARENTHESIZED_HPP__

#include "nodes/node_binary.hpp"

/**
 * @brief A seabow node that represents a parenthesized seabow expression.
*/
class NodeParenthesized: public Node {
private:
    Node *expression; // The parenthesized seabow expression.

public:
    /**
     * @brief Construct a new seabow nodeparenthesized.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
     * @param expression The parenthesized seabow expression.
    */
    NodeParenthesized(sbw_ulong line, sbw_ulong column, Node *expression);

    /**
     * @brief Destroy a seabow nodeparenthesized.
    */
    inline virtual ~NodeParenthesized() {}

    /**
     * @brief Get the seabow node's type of a seabow nodeparenthesized (i.e. SBW_NODE_PARENTHESIZED).
     * @return SBW_NODE_PARENTHESIZED.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_PARENTHESIZED; }
    
    /**
     * @brief Get the parenthesized seabow expression.
     * @return The parenthesized seabow expression.
    */
    inline Node *Expression(sbw_none) const { return this->expression; }
};

#endif // __SEABOW_NODE_PARENTHESIZED_HPP__