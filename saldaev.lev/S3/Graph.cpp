#include "Graph.hpp"

saldaev::Graph::Graph():
  edges(10, Hasher(), KeyEqual()),
  vertexes(0)
{}
