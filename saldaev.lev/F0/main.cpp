#include <fstream>
#include <iostream>
#include "comandHandlers.hpp"

int main()
{
  using Command = void (*)(std::istream &, std::ostream &, saldaev::Baskets &, saldaev::Recipes &);
  using Commands = saldaev::HashTable< std::string, Command, std::hash< std::string >, std::equal_to< std::string > >;

  Commands commands(10, std::hash< std::string >(), std::equal_to< std::string >());
  commands.add("help", saldaev::handleHelp);
  commands.add("basket_list", saldaev::handleBasket_list);
  commands.add("basket_create", saldaev::handleBasket_create);
  commands.add("basket_delete", saldaev::handleBasket_delete);
  commands.add("basket_copy", saldaev::handleBasket_copy);
  commands.add("merge", saldaev::handleMerge);
  commands.add("prod_add", saldaev::handleProd_add);
  commands.add("prod_remove", saldaev::handleProd_remove);
  commands.add("prod_list", saldaev::handleProd_list);
  commands.add("recipe_add", saldaev::handleRecipe_add);
  commands.add("recipe_remove", saldaev::handleRecipe_remove);
  commands.add("show", saldaev::handleShow);
  commands.add("add", saldaev::handleAdd);
  commands.add("remove", saldaev::handleRemove);
  commands.add("cook", saldaev::handleCook);
  commands.add("cookable", saldaev::handleCookable);
  commands.add("show_recipe", saldaev::handleShow_recipe);
  commands.add("need_for", saldaev::handleNeed_for);
  commands.add("where_used", saldaev::handleWhere_used);

  std::ifstream file;
  file.open("basketsSaveFile.txt");
  if (!file.is_open()) {
    std::cerr << "failed to open baskets save file\n";
  }

  saldaev::Baskets baskets(5, std::hash< std::string >{}, std::equal_to< std::string >{});

  std::string bName;
  while (file >> bName) {
    saldaev::Basket b(saldaev::BASKET_CAP, std::hash< std::string >{}, std::equal_to< std::string >{});
    while (file.peek() != '\n') {
      std::string item;
      size_t qty;
      if (!(file >> item >> qty)) {
        std::cout << " - baskets save file is corrupted\n";
        file.setstate(std::ios::failbit);
        baskets.clear();
        break;
      }
      b.add(item, qty);
    }
    baskets.add(bName, b);
  }
  file.close();

  file.open("graphSaveFile.txt");
  if (!file.is_open()) {
    std::cerr << "failed to open graph save file\n";
  }

  saldaev::Recipes recipes;

  while (file.peek() != '\n') {
    std::string vName;
    size_t n;
    if (!(file >> vName >> n)) {
      std::cout << " - graph save file is corrupted\n";
      file.setstate(std::ios::failbit);
      recipes.clear();
      break;
    }
    recipes.addVertex({vName, n}, vName);
  }

  std::string vName1;
  std::string vName2;
  size_t weight;
  while (file >> vName1) {
    if (!(file >> vName2 >> weight)) {
      std::cout << " - graph save file is corrupted\n";
      file.setstate(std::ios::failbit);
      recipes.clear();
      break;
    }
    recipes.addEdge(vName1, vName2, weight);
  }

  std::string cmd = "";
  while (std::cin >> cmd) {
    if (commands.has(cmd)) {
      try {
        commands.get(cmd)(std::cin, std::cout, baskets, recipes);
      } catch (const std::exception &e) {
        std::cout << "failed to process the command (" << e.what() << ")\n";
      }
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    } else {
      std::cout << " - no such command\n";
    }
  }
}
