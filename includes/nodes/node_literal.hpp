/**
 * @file node_literal.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a seabow value literal (i.e. string, number, ...).
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_LITERAL_HPP__
#define __SEABOW_NODE_LITERAL_HPP__

#include "nodes/node_parenthesized.hpp"

/**
 * @brief A seabow node that represents a seabow value literal (i.e. string, number, ...).
*/
class NodeLiteral: public Node {
private:
    SBW_Value *value; // The seabow value literal of the seabow nodeliteral.

public:
    /**
     * @brief Construct a new seabow nodeliteral.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
     * @param value The seabow value literal of the seabow nodeliteral.
    */
    NodeLiteral(sbw_ulong line, sbw_ulong column, SBW_Value *value);

    /**
     * @brief Destroy a seabow nodeliteral.
    */
    inline virtual ~NodeLiteral() {}

    /**
     * @brief Get the seabow node's type of a seabow nodeliteral (i.e. SBW_NODE_LITERAL).
     * @return SBW_NODE_LITERAL.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_LITERAL; }
    
    /**
     * @brief Get the seabow value literal of the seabow nodeliteral.
     * @return The seabow value literal.
    */
    inline SBW_Value *Value(sbw_none) const { return this->value; }
};

#endif // __SEABOW_NODE_LITERAL_HPP__