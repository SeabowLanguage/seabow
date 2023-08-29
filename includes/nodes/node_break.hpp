/**
 * @file node_break.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a break loop controller.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_BREAK_HPP__
#define __SEABOW_NODE_BREAK_HPP__

#include "nodes/node_return.hpp"

/**
 * @brief A seabow node that represents a break loop controller.
*/
class NodeBreak : public Node {
public:
    /**
     * @brief Construct a new seabow nodebreak
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
    */
    NodeBreak(sbw_ulong line, sbw_ulong column);

    /**
     * @brief Destroy a seabow nodebreak.
    */
    inline virtual ~NodeBreak(sbw_none) {}

    /**
     * @brief Get the seabow node's type of a seabow nodebreak (i.e. SBW_NODE_BREAK).
     * @return SBW_NODE_BREAK.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_BREAK; }
};

#endif // __SEABOW_NODE_BREAK_HPP__