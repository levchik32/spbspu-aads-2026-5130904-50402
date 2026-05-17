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
