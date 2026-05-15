#include "Graph.hpp"
#include <fstream>
#include <iostream>

using GraphStorage =
    saldaev::HashTable< std::string, saldaev::Graph *, std::hash< std::string >, std::equal_to< std::string > >;

int main(int argc, char *argv[])
{
  using Command = void (*)(std::istream &in, std::ostream &out, GraphStorage &graphs);
  using Commands = saldaev::HashTable< std::string, Command, std::hash< std::string >, std::equal_to< std::string > >;

  Commands commands(9, std::hash< std::string >(), std::equal_to< std::string >());

  if (argc != 2) {
    std::cerr << "wrong amoung of arguments";
    return 1;
  }

  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open()) {
    std::cerr << "failed to open file";
    return 1;
  }

  std::istream &input = file;

  GraphStorage graphs(10, std::hash< std::string >(), std::equal_to< std::string >());

  std::string name = "";
  size_t n = 0;
  while (input >> name && input >> n) {
    if (graphs.has(name)) {
      std::cerr << "problems with the file content";
      return 2;
    }
    saldaev::Graph *g = new saldaev::Graph();

    for (size_t i = 0; i < n; ++i) {
      std::string v1 = "";
      std::string v2 = "";
      size_t w = 0;
      if (!(input >> v1 && input >> v2 && input >> w)) {
        std::cerr << "problems with the file content";
        return 2;
      }

      if (!(g->hasVertex(v1))) {
        g->addVertex(v1);
      }
      if (!(g->hasVertex(v2))) {
        g->addVertex(v2);
      }
      g->addEdge(v1, v2, w);
    }

    graphs.add(name, g);
  }

  file.close();

  std::string cmd = "";
  while (std::cin >> cmd) {
    if (commands.has(cmd)) {
      commands.get(cmd)(std::cin, std::cout, graphs);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  auto it = graphs.begin();
  while (it != graphs.end()) {
    delete it->second;
    ++it;
  }
}

template < class T >
void sort(saldaev::Vector< T > &v)
{
  if (v.getSize() < 2)
    return;

  bool swapped = true;
  while (swapped) {
    swapped = false;
    auto curr = v.begin();
    auto next = curr;
    ++next;
    while (next != v.end()) {
      if (*next < *curr) {
        std::swap(*curr, *next);
        swapped = true;
      }
      ++curr;
      ++next;
    }
  }
}

void handleGraphs(std::istream &in, std::ostream &out, GraphStorage &graphs)
{
  saldaev::Vector< std::string > g(0);
  auto it = graphs.begin();
  while (it != graphs.end()) {
    g.pushBack(it->first);
    ++it;
  }
  sort(g);
  for (size_t i = 0; i < g.getSize(); ++i) {
    out << g[i] << '\n';
  }
}

void handleVertexes(std::istream &in, std::ostream &out, GraphStorage &graphs)
{
  std::string name = "";
  in >> name;
  if (!(graphs.has(name))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  saldaev::Vector< std::string > v = (graphs.get(name))->vertices();
  sort(v);
  for (size_t i = 0; i < v.getSize(); ++i) {
    out << v[i] << '\n';
  }
}

void handleOutbound(std::istream &in, std::ostream &out, GraphStorage &graphs)
{
  std::string name = "";
  in >> name;
  if (!(graphs.has(name))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::string vertex = "";
  in >> vertex;
  if (!(graphs.get(name)->hasVertex(vertex))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  saldaev::Vector< std::pair< std::string, saldaev::Vector< size_t > > > v = (graphs.get(name))->outgoingEdges(vertex);
  for (size_t i = 0; i < v.getSize(); ++i) {
    out << v[i].first;
    saldaev::Vector< size_t > w = v[i].second;
    for (size_t j = 0; j < w.getSize(); ++j) {
      out << ' ' << w[j];
    }
    out << '\n';
  }
}

void handleInbound(std::istream &in, std::ostream &out, GraphStorage &graphs)
{
  std::string name = "";
  in >> name;
  if (!(graphs.has(name))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::string vertex = "";
  in >> vertex;
  if (!(graphs.get(name)->hasVertex(vertex))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  saldaev::Vector< std::pair< std::string, saldaev::Vector< size_t > > > v = (graphs.get(name))->incomingEdges(vertex);
  for (size_t i = 0; i < v.getSize(); ++i) {
    out << v[i].first;
    saldaev::Vector< size_t > w = v[i].second;
    for (size_t j = 0; j < w.getSize(); ++j) {
      out << ' ' << w[j];
    }
    out << '\n';
  }
}

void handleBind(std::istream &in, std::ostream &out, GraphStorage &graphs)
{
  std::string gName;
  in >> gName;
  if (!(graphs.has(gName))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  saldaev::Graph *graph = graphs.get(gName);

  std::string vName1, vName2;
  size_t w = 0;
  if (!(in >> vName1 >> vName2 >> w)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!(graph->hasVertex(vName1))) {
    graph->addVertex(vName1);
  }
  if (!(graph->hasVertex(vName2))) {
    graph->addVertex(vName2);
  }

  graph->addEdge(vName1, vName2, w);
}

void handleCut(std::istream &in, std::ostream &out, GraphStorage &graphs)
{
  std::string gName;
  in >> gName;
  if (!(graphs.has(gName))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  saldaev::Graph *graph = graphs.get(gName);

  std::string vName1, vName2;
  size_t w = 0;
  if (!(in >> vName1 >> vName2 >> w)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!(graph->hasVertex(vName1)) || !(graph->hasVertex(vName2))) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!(graph->hasEdge(vName1, vName2))) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    graph->removeEdge(vName1, vName2, w);
  } catch (const std::logic_error &) {
    out << "<INVALID COMMAND>\n";
  }
}
