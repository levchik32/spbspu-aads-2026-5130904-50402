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

  saldaev::Recipes recipes;
  saldaev::Baskets baskets(5, std::hash< std::string >{}, std::equal_to< std::string >{});

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

  auto it = baskets.begin();
  while (it != baskets.end()) {
    delete it->second;
    ++it;
  }
}
