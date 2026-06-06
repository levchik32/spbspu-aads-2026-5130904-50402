#include <fstream>
#include <iostream>
#include "BSTree.hpp"

using Dataset = saldaev::BSTree< long long, std::string >;
using Datasets = saldaev::BSTree< std::string, Dataset >;

int main(int argc, char *argv[])
{
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

  Datasets datasets;
  std::string name;
  while (file >> name) {
    Dataset dataset;
    long long key = 0;
    std::string value;
    while (file >> key) {
      if (file >> value) {
        dataset.push(key, value);
      } else {
        std::cerr << "missing value";
        return 1;
      }
    }
    if (!(file.eof())) {
      file.clear();
    }
    datasets.push(name, dataset);
  }

  file.close();
}