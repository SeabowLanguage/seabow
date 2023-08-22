#ifndef __SEABOW_NODE_HPP__
#define __SEABOW_NODE_HPP__

#include "values/value_error.hpp"

enum sbw_node_type : sbw_ubyte {
    SBW_NODE_UNKNOWN = 0xff, SBW_NODE_BAD = 0xfe,

    SBW_NODE_COMPOUND = 0x00
};

class Node {
private:
    sbw_ulong line, column;

public:
    Node(sbw_ulong l, sbw_ulong c);
    inline virtual ~Node(sbw_none) {}

    inline sbw_ulong Line(sbw_none) { return this->line; }
    inline sbw_ulong Column(sbw_none) { return this->column; }
    inline virtual sbw_node_type Type(sbw_none) const { return SBW_NODE_UNKNOWN; }
};

#endif // __SEABOW_NODE_HPP__