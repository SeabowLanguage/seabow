/**
 * @file node_return.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a return function controller.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_RETURN_HPP__
#define __SEABOW_NODE_RETURN_HPP__

#include "nodes/node_compound.hpp"

/**
 * @brief A seabow node that represents a return function controller.
*/
class NodeReturn : public Node {
private:
    Node *expression; // The seabow expression to return.

public:
    /**
     * @brief Construct a new seabow nodereturn.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
     * @param expression The seabow expression to return.
    */
    NodeReturn(sbw_ulong line, sbw_ulong column, Node *expression);

    /**
     * @brief Destroy a seabow nodereturn.
    */
    inline virtual ~NodeReturn(sbw_none) {}

    /**
     * @brief Get the seabow node's type of a seabow nodereturn (i.e. SBW_NODE_RETURN).
     * @return SBW_NODE_RETURN.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_RETURN; }

    /**
     * @brief Get the seabow expression to return.
     * @return The seabow expression to return.
    */
    inline Node *Expression(sbw_none) const { return this->expression; }
};

#endif // __SEABOW_NODE_RETURN_HPP__