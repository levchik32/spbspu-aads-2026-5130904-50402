#include "Graph.hpp"

saldaev::Graph::Graph():
  edges(10, Hasher(), KeyEqual()),
  vertexes(0)
{}

saldaev::Vector< std::string > saldaev::Graph::vertices() const
{
  return vertexes;
}

bool saldaev::Graph::hasVertex(const std::string &name) const
{
  for (size_t i = 0; i < vertexes.getSize(); ++i) {
    if (vertexes[i] == name) {
      return true;
    }
  }
  return false;
}

void saldaev::Graph::addVertex(const std::string &name)
{
  for (size_t i = 0; i < vertexes.getSize(); ++i) {
    if (vertexes[i] == name) {
      throw std::logic_error("ununique name");
    }
  }
  vertexes.pushBack(name);
}
