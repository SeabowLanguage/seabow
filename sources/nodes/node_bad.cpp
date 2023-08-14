#include "node_bad.hpp"

NodeBad::NodeBad(sbw_ulong line, sbw_ulong column, SBW_ValueError *err)
    : Node(line, column)
{
    this->error = err;
}