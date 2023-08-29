/**
 * @file node_continue.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a continue loop controller.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_CONTINUE_HPP__
#define __SEABOW_NODE_CONTINUE_HPP__

#include "nodes/node_break.hpp"

/**
 * @brief A seabow node that represents a continue loop controller.
*/
class NodeContinue : public Node {
public:
    /**
     * @brief Construct a new seabow nodecontinue.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
    */
    NodeContinue(sbw_ulong line, sbw_ulong column);

    /**
     * @brief Destroy a seabow nodecontinue.
    */
    inline virtual ~NodeContinue(sbw_none) {}
    
    /**
     * @brief Get the seabow node's type of a seabow nodecontinue (i.e. SBW_NODE_CONTINUE).
     * @return SBW_NODE_CONTINUE.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_CONTINUE; }
};

#endif // __SEABOW_NODE_CONTINUE_HPP__