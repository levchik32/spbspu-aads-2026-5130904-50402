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

bool saldaev::Graph::hasEdge(const std::string &from, const std::string &to) const
{
  return edges.has({from, to});
}

void saldaev::Graph::addEdge(const std::string &from, const std::string &to, size_t weight)
{
  if (!hasEdge(from, to)) {
    edges.add({from, to}, Vector< size_t >(1));
  }
  edges.at({from, to}).pushBack(weight);
}

void saldaev::Graph::removeEdge(const std::string &from, const std::string &to, size_t weight)
{
  Vector< size_t > &v = edges.at({from, to});
  for (size_t i = 0; i < v.getSize(); ++i) {
    if (v[i] == weight) {
      v.erase(i);
      if (v.isEmpty()) {
        edges.remove({from, to});
      }
      return;
    }
  }
  throw std::logic_error("no such edge");
}

const saldaev::Vector< std::pair< std::string, size_t > > saldaev::Graph::outgoingEdges(const std::string &name) const
{
  Vector< std::pair< std::string, size_t > > ret(0);

  for (size_t i = 0; i < vertexes.getSize(); ++i) {
    if (hasEdge(name, vertexes[i])) {
      const Vector< size_t > &v = edges.get({name, vertexes[i]});
      for (size_t j = 0; j < v.getSize(); ++j) {
        ret.pushBack({vertexes[i], v[j]});
      }
    }
  }
  return ret;
}

const saldaev::Vector< std::pair< std::string, size_t > > saldaev::Graph::incomingEdges(const std::string &name) const
{
  Vector< std::pair< std::string, size_t > > ret(0);

  for (size_t i = 0; i < vertexes.getSize(); ++i) {
    if (hasEdge(vertexes[i], name)) {
      const Vector< size_t > &v = edges.get({vertexes[i], name});
      for (size_t j = 0; j < v.getSize(); ++j) {
        ret.pushBack({vertexes[i], v[j]});
      }
    }
  }
  return ret;
}
