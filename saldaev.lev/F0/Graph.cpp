#include "Graph.hpp"

template < class T, class S >
saldaev::Graph< T, S >::Vertex::Vertex(T d):
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

  Vertex *nV = new Vertex(data);
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
