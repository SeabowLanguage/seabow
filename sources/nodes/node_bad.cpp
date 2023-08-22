#include "nodes/node_bad.hpp"

NodeBad::NodeBad(SBW_ValueError *err)
    : Node(err->Line(), err->Column())
{
    this->error = err;
}