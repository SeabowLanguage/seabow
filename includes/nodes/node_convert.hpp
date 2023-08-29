/**
 * @file node_convert.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a seabow type conversion.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_CONVERT_HPP__
#define __SEABOW_NODE_CONVERT_HPP__

#include "nodes/node_continue.hpp"

/**
 * @brief A seabow node that represents a seabow type conversion.
*/
class NodeConvert : public Node {
private:
    Node *expression; // The expression to convert to another type.

public:
    /**
     * @brief Construct a new seabow nodeconvert.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
     * @param expression The expression to convert to another type.
    */
    NodeConvert(sbw_ulong line, sbw_ulong column, Node *expression);

    /**
     * @brief Destroy a seabow nodeconvert.
    */
    inline virtual ~NodeConvert(sbw_none) {}

    /**
     * @brief Get the seabow node's type of a seabow nodeconverte (i.e. SBW_NODE_CONVERT).
     * @return SBW_NODE_CONVERT.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_CONVERT; }
    
    /**
     * @brief Get the seabow expression to convert.
     * @return The seabow expression to convert.
    */
    inline Node *Expression(sbw_none) const { return this->expression; }
};

#endif // __SEABOW_NODE_CONVERT_HPP__