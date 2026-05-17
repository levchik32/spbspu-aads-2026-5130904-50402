#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "../common/HashTable.hpp"
#include "../common/vector.hpp"

namespace saldaev
{
  template < class T, class S >
  struct Graph
  {
  private:
    struct Hasher
    {
      size_t operator()(const std::string &p) const;
    };

    struct KeyEqual
    {
      bool operator()(const std::string &st, const std::string &nd) const;
    };

    struct Edge;
    struct Vertex
    {
      T data;
      Vector< Edge * > outgoing_edges;
      Vector< Edge * > incoming_edges;

      Vertex(T d);
    };

    struct Edge
    {
      S data;
      Vertex *from;
      Vertex *to;

      Edge(S d, Vertex *f, Vertex *t);
    };

    HashTable< std::string, Vertex *, Hasher, KeyEqual > vertexes;
    Vector< Edge * > edges;

    void removeEdgesOf(Vertex *);
    Vertex *findVertex(const std::string &id);

  public:
    Graph();
    Graph(const Graph &) = delete;
    Graph(Graph &&) = delete;
    Graph &operator=(const Graph &) = delete;
    Graph &operator=(Graph &&) = delete;
    ~Graph();

    Vector< std::string > vertexIds() const;
    bool hasVertex(const std::string &id) const;
    void addVertex(const T &data, const std::string &id);
    void removeVertex(const std::string &id);
    void setVertexData(const std::string &id, const T &data);
    T getVertexData(const std::string &id) const;

    Vector< std::pair< std::string, std::string > > edgesIds() const;
    bool hasEdge(const std::string &from, const std::string &to) const;
    void addEdge(const std::string &from, const std::string &to, const S &data);
    void removeEdge(const std::string &from, const std::string &to);
    void setEdgeData(const std::string &from, const std::string &to, const S &data);
    S getEdgeData(const std::string &from, const std::string &to) const;

    size_t indegree(const std::string &id) const;
    size_t outdegree(const std::string &id) const;
    size_t degree(const std::string &id) const;
    const Vector< std::string > outgoingEdges(const std::string &id) const;
    const Vector< std::string > incomingEdges(const std::string &id) const;

    size_t countVertexes() const;
    size_t countEdges() const;

    void clearEdges();
    void clear();
  };
}

#endif
