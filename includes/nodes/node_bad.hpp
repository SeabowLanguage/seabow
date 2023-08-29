/**
 * @file node_bad.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a syntax error.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_BAD_HPP__
#define __SEABOW_NODE_BAD_HPP__

#include "node.hpp"

/**
 * @brief A seabow node that represents a syntax error.
*/
class NodeBad: public Node {
private:
    SBW_ValueError *error; // The error of the node.

public:
    /**
     * @brief Construct a new seabow nodebad.
     * @param err The error of the node.
    */
    NodeBad(SBW_ValueError *err);

    /**
     * @brief Destroy a seabow nodebad.
    */
    inline virtual ~NodeBad(sbw_none) {}

    /**
     * @brief Get the error of the nodebad.
     * @return The error of the nodebad.
    */
    inline SBW_ValueError *Error(sbw_none) { return this->error; }
    
    /**
     * @brief Get the seabow node's type of a seabow nodebad (i.e. SBW_NODE_BAD).
     * @return SBW_NODE_BAD.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_BAD; }
};

#endif // __SEABOW_NODE_BAD_HPP__