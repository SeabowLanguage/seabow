/**
 * @file node.hpp
 * @author LucaStarz
 * @brief Description of the base of any seabow nodes that represents a simple statement.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_HPP__
#define __SEABOW_NODE_HPP__

#include "core/lexer.hpp"

/**
 * @brief Specify all kind of seabow nodes.
*/
enum sbw_node_type : sbw_ubyte {
    SBW_NODE_UNKNOWN = 0xff, SBW_NODE_BAD = 0xfe,

    SBW_NODE_COMPOUND = 0x00,

    SBW_NODE_RETURN = 0x0a, SBW_NODE_BREAK = 0x0b, SBW_NODE_CONTINUE = 0x0c,

    SBW_NODE_CONVERT = 0x10, SBW_NODE_PARENTHESIZED = 0x11, SBW_NODE_LITERAL = 0x12,

    SBW_NODE_UNARY = 0x1a, SBW_NODE_BINARY = 0x1b
};

/**
 * @brief A seabow node that represents a simple seabow statement.
*/
class Node {
private:
    sbw_ulong line, column; // The position of the node in its file.

public:
    /**
     * @brief Construct a new seabow node.
     * @param l The line position of the node in its file.
     * @param c The column position of the node in its file.
    */
    Node(sbw_ulong l, sbw_ulong c);

    /**
     * @brief Destroy a seabow node.
    */
    inline virtual ~Node(sbw_none) {}

    /**
     * @brief Get the line position of the node in its file.
     * @return The line position of the node in its file.
    */
    inline sbw_ulong Line(sbw_none) { return this->line; }

    /**
     * @brief Get the column position of the node in its file.
     * @return The column position of the node in its file.
    */
    inline sbw_ulong Column(sbw_none) { return this->column; }

    /**
     * @brief Get the seabow node's type of a base of seabow node (i.e. SBW_NODE_UNKNOWN).
     * @return SBW_NODE_UNKNOWN.
    */
    inline virtual sbw_node_type Type(sbw_none) const { return SBW_NODE_UNKNOWN; }
};

#endif // __SEABOW_NODE_HPP__