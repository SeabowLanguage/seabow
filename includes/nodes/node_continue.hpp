#ifndef __SEABOW_NODE_CONTINUE_HPP__
#define __SEABOW_NODE_CONTINUE_HPP__

#include "nodes/node_break.hpp"

class NodeContinue : public Node {
public:
    NodeContinue(sbw_ulong line, sbw_ulong column);
    inline virtual ~NodeContinue(sbw_none) {}
    
    inline sbw_node_type Type(sbw_none) const { return SBW_NODE_CONTINUE; }
};

#endif // __SEABOW_NODE_CONTINUE_HPP__