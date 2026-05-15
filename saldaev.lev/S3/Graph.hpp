#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "../common/vector.hpp"
#include "HashTable.hpp"
#include <string>

namespace saldaev
{
  struct Graph
  {
  private:
    struct Hasher
    {
      size_t operator()(const std::pair< std::string, std::string > &p) const;
    };

    struct KeyEqual
    {
      bool operator()(const std::pair< std::string, std::string > &st,
                      const std::pair< std::string, std::string > &nd) const;
    };

    HashTable< std::pair< std::string, std::string >, Vector< size_t >, Hasher, KeyEqual > edges;
    Vector< std::string > vertexes;

  public:
    Graph();

    Vector< std::string > vertices() const;
    bool hasVertex(const std::string &name) const;
    void addVertex(const std::string &name);

    bool hasEdge(const std::string &from, const std::string &to) const;
    void addEdge(const std::string &from, const std::string &to, size_t weight);
    void removeEdge(const std::string &from, const std::string &to, size_t weight);

    const Vector< std::pair< std::string, Vector< size_t > > > outgoingEdges(const std::string &name) const;
    const Vector< std::pair< std::string, Vector< size_t > > > incomingEdges(const std::string &name) const;

    void clearEdges();
    void clear();
  };
}

#endif
