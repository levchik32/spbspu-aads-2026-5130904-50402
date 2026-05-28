#include <fstream>
#include <iostream>
#include "comandHandlers.hpp"

int main()
{
  using Command = void (*)(std::istream &, std::ostream &, saldaev::Baskets &, saldaev::Recipes &);
  using Commands = saldaev::HashTable< std::string, Command, std::hash< std::string >, std::equal_to< std::string > >;

  Commands commands(10, std::hash< std::string >(), std::equal_to< std::string >());
  commands.add("help", saldaev::handleHelp);

  saldaev::Recipes recipes;
  saldaev::Baskets baskets(5, std::hash< std::string >{}, std::equal_to< std::string >{});

  std::string cmd = "";
  while (std::cin >> cmd) {
    if (commands.has(cmd)) {
      commands.get(cmd)(std::cin, std::cout, baskets, recipes);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  auto it = baskets.begin();
  while (it != baskets.end()) {
    delete it->second;
    ++it;
  }
}
