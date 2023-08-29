/**
 * @file node_compound.hpp
 * @author LucaStarz
 * @brief Description of a specific seabow node that represents a list of multiple seabow nodes.
 * @date 2023-08-29
 * @copyright Copyright (c) 2023
*/

#ifndef __SEABOW_NODE_COMPOUND_HPP__
#define __SEABOW_NODE_COMPOUND_HPP__

#include "node_bad.hpp"

/**
 * @brief A seabow node that represents a list of seabow nodes.
*/
class NodeCompound: public Node {
private:
    std::vector<Node*> statements; // The list of seabow nodes contained by the seabow nodecompound.

public:
    /**
     * @brief Construct a new seabow nodecompound.
     * @param line The line position of the node in its file.
     * @param column The column position of the node in its file.
     * @param stats The statements (i.e. seabow nodes) contained in the scope of the seabow nodecompound.
    */
    NodeCompound(sbw_ulong line, sbw_ulong column, std::vector<Node*> stats);

    /**
     * @brief Destroy a seabow nodecompound.
    */
    inline virtual ~NodeCompound(sbw_none) {}

    /**
     * @brief Get the seabow node's type of a seabow nodecompound (SBW_NODE_COMPOUND).
     * @return SBW_NODE_COMPOUND.
    */
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_COMPOUND; }
    
    /**
     * @brief Get the statements (i.e. seabow nodes) contained in the scope of the seabowcompound.
     * @return The statements / seabow nodes.
    */
    inline std::vector<Node*> Statements(sbw_none) const { return this->statements; }
};

#endif // __SEABOW_NODE_COMPOUND_HPP__