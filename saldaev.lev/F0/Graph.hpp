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
      std::string id;
      T data;
      Vector< Edge * > outgoing_edges;
      Vector< Edge * > incoming_edges;

      Vertex(std::string i, T d);
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

template < class T, class S >
saldaev::Graph< T, S >::Vertex::Vertex(std::string i, T d):
  id(i),
  data(d),
  outgoing_edges(0),
  incoming_edges(0)
{}

template < class T, class S >
saldaev::Graph< T, S >::Edge::Edge(S d, Vertex *f, Vertex *t):
  data(d),
  from(f),
  to(t)
{}

template < class T, class S >
saldaev::Graph< T, S >::Graph():
  vertexes(31, Hasher(), KeyEqual()),
  edges(0)
{}

template < class T, class S >
saldaev::Graph< T, S >::~Graph()
{
  for (size_t i = 0; i < edges.getSize(); ++i) {
    delete edges[i];
  }

  auto it = vertexes.begin();
  while (it != vertexes.end()) {
    delete it->second;
    ++it;
  }
}

template < class T, class S >
void saldaev::Graph< T, S >::removeEdgesOf(Vertex *v)
{
  for (size_t i = 0; i < v->incoming_edges.getSize(); ++i) {
    Edge *e = v->incoming_edges[i];
    e->from->outgoing_edges.remove(e);
    edges.remove(e);
    delete e;
  }
  for (size_t i = 0; i < v->outgoing_edges.getSize(); ++i) {
    Edge *e = v->outgoing_edges[i];
    e->to->incoming_edges.remove(e);
    edges.remove(e);
    delete e;
  }

  v->incoming_edges.clear();
  v->outgoing_edges.clear();
}

template < class T, class S >
saldaev::Vector< std::string > saldaev::Graph< T, S >::vertexIds() const
{
  Vector< std::string > ret(0);
  auto it = vertexes.begin();
  while (it != vertexes.end()) {
    ret.pushBack(it->first);
    ++it;
  }
  return ret;
}

template < class T, class S >
bool saldaev::Graph< T, S >::hasVertex(const std::string &id) const
{
  return vertexes.has(id);
}

template < class T, class S >
void saldaev::Graph< T, S >::addVertex(const T &data, const std::string &id)
{
  if (vertexes.has(id)) {
    throw std::invalid_argument("ununique vertex name");
  }

  Vertex *nV = new Vertex(id, data);
  vertexes.add(id, nV);
}

template < class T, class S >
void saldaev::Graph< T, S >::removeVertex(const std::string &id)
{
  if (!(vertexes.has(id))) {
    throw std::out_of_range("no vertex with this name");
  }

  Vertex *v = vertexes.get(id);
  removeEdgesOf(v);

  delete v;
  vertexes.remove(id);
}

template < class T, class S >
void saldaev::Graph< T, S >::setVertexData(const std::string &id, const T &data)
{
  if (!(vertexes.has(id))) {
    throw std::logic_error("no vertex with this name");
  }
  vertexes.get(id)->data = data;
}

template < class T, class S >
T saldaev::Graph< T, S >::getVertexData(const std::string &id) const
{
  return vertexes.get(id)->data;
}

#endif
