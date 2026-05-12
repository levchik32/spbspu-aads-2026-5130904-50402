#include "Graph.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
  using GraphStorage =
      saldaev::HashTable< std::string, saldaev::Graph *, std::hash< std::string >, std::equal_to< std::string > >;

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

  auto it = graphs.begin();
  while (it != graphs.end()) {
    delete it->second;
    ++it;
  }
}
