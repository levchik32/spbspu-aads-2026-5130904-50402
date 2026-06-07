#ifndef COMANDHANDLERS_HPP
#define COMANDHANDLERS_HPP
#include <iostream>
#include <limits>
#include "Graph.hpp"

namespace saldaev
{
  const size_t BASKET_CAP = 11;

  using Recipes = Graph< std::pair< std::string, size_t >, size_t >;
  using Basket = HashTable< std::string, size_t, std::hash< std::string >, std::equal_to< std::string > >;
  using Baskets = HashTable< std::string, Basket *, std::hash< std::string >, std::equal_to< std::string > >;

  const std::pair< std::string, std::string > HELP_DATA[] = {
      {"help", "help <команда> - выводит описание указанной команды."},
      {"basket_list", "basket_list - выводит список доступных корзин."},
      {"basket_create", "basket_create <name> - создаёт корзину с именем <name>."},
      {"basket_delete", "basket_delete <name> - удаляет корзину <name>."},
      {"basket_copy", "basket_copy <name> <new_name> - копирует корзину <name> в <new_name>."},
      {"merge", "merge <name1> <name2> <new_name> - создаёт корзину <new_name> как объединение <name1> и <name2>."},
      {"show", "show <b_name> - выводит содержимое корзины <b_name>."},
      {"add", "add <b_name> <item> <qty> - добавляет продукт/блюдо <item> в количестве <qty> в корзину <b_name>."},
      {"remove", "remove <b_name> <item> <qty> - удаляет <qty> элемента <item> из корзины <b_name>."},
      {"cook",
       "cook <b_name> <dish> <qty> - приготавливает <dish> в количестве <qty> из ингредиентов в корзине <b_name> "
       "(максимально возможное число порций при нехватке)."},
      {"recipe_add", "recipe_add <dish> <qty> <i_qty> (<item> <qty>) x i_qty - создаёт рецепт блюда <dish>."},
      {"recipe_remove",
       "recipe_remove <dish> - удаляет рецепт блюда <dish>, если оно не используется в других рецептах."},
      {"prod_add", "prod_add <name> - добавляет базовый продукт <name>."},
      {"prod_remove", "prod_remove <name> - удаляет базовый продукт <name>, если он не используется в рецептах."},
      {"prod_list", "prod_list <base|derived|all> - выводит список базовых, составных или всех продуктов."},
      {"cookable", "cookable <b_name> <direct|full> - выводит блюда, которые можно приготовить из корзины <b_name> "
                   "(direct без цепочек, full с цепочками)."},
      {"show_recipe", "show_recipe <dish> <depth> - выводит рецепт блюда <dish> с глубиной раскрытия <depth>."},
      {"need_for", "need_for <dish> <qty> <b_name> - создаёт корзину <b_name> с продуктами для приготовления <dish> в "
                   "количестве <qty>."},
      {"where_used", "where_used <item> - выводит список блюд, в которых используется <item>."}};

  void handleHelp(std::istream &in, std::ostream &out, Baskets &, Recipes &)
  {
    std::string com;
    in >> com;
    if (in.eof()) {
      return;
    }

    size_t size = sizeof(HELP_DATA) / sizeof(HELP_DATA[0]);
    for (size_t i = 0; i < size; ++i) {
      if (HELP_DATA[i].first == com) {
        out << " - " << HELP_DATA[i].second << '\n';
        return;
      }
    }
    out << " - no such command\n";
  }

  void handleBasket_list(std::istream &, std::ostream &out, Baskets &baskets, Recipes &)
  {
    auto it = baskets.begin();
    if (it == baskets.end()) {
      out << " - no available baskets\n";
      return;
    }
    while (it != baskets.end()) {
      out << " - " << it->first << '\n';
      ++it;
    }
  }

  void handleBasket_create(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &)
  {
    std::string name;
    in >> name;
    if (in.eof()) {
      return;
    }

    if (baskets.has(name)) {
      out << " - name is not unique\n";
      return;
    }

    Basket *b = new Basket(BASKET_CAP, std::hash< std::string >{}, std::equal_to< std::string >{});
    try {
      baskets.add(name, b);
    } catch (...) {
      delete b;
      throw;
    }
  }

  void handleBasket_delete(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &)
  {
    std::string name;
    in >> name;
    if (in.eof()) {
      return;
    }

    if (!(baskets.has(name))) {
      out << " - no such basket\n";
      return;
    }

    delete baskets.get(name);
    baskets.remove(name);
  }

  void handleBasket_copy(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &)
  {
    std::string name, name2;
    in >> name >> name2;
    if (in.eof()) {
      return;
    }

    if (!(baskets.has(name))) {
      out << " - no such basket\n";
      return;
    }
    if (baskets.has(name2)) {
      out << " - new name is not unique\n";
      return;
    }

    Basket *b = new Basket(*(baskets.get(name)));
    try {
      baskets.add(name2, b);
    } catch (...) {
      delete b;
      throw;
    }
  }

  void handleMerge(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &)
  {
    std::string name1, name2, new_name;
    in >> name1 >> name2 >> new_name;
    if (in.eof()) {
      return;
    }

    if (!(baskets.has(name1)) || !(baskets.has(name2))) {
      out << " - at least one of the baskets doesn't exist\n";
      return;
    }
    if (baskets.has(new_name)) {
      out << " - new name is not unique\n";
      return;
    }

    Basket *b = new Basket(*(baskets.get(name1)));
    Basket *b2 = baskets.get(name2);
    auto it = b2->begin();
    try {
      while (it != b2->end()) {
        if (b->has(it->first)) {
          b->at(it->first) += it->second;
        } else {
          b->add(it->first, it->second);
        }
        ++it;
      }

      baskets.add(new_name, b);
    } catch (...) {
      delete b;
      throw;
    }
  }

  void handleProd_add(std::istream &in, std::ostream &out, Baskets &, Recipes &recipes)
  {
    std::string name;
    in >> name;
    if (in.eof()) {
      return;
    }

    if (recipes.hasVertex(name)) {
      out << " - name is not unique\n";
      return;
    }

    recipes.addVertex({name, 0}, name);
  }

  void handleProd_remove(std::istream &in, std::ostream &out, Baskets &, Recipes &recipes)
  {
    std::string name;
    in >> name;
    if (in.eof()) {
      return;
    }

    if (!(recipes.hasVertex(name))) {
      out << " - no such ingredient/dish\n";
      return;
    }
    if (recipes.outdegree(name)) {
      out << " - cannot remove this product: it is used as an ingredient\n";
      return;
    }

    recipes.removeVertex(name);
  }

  void handleProd_list(std::istream &in, std::ostream &out, Baskets &, Recipes &recipes)
  {
    std::string opt;
    in >> opt;
    if (in.eof()) {
      return;
    }

    bool base = false;
    bool derived = false;
    if (opt == "base") {
      base = true;
    } else if (opt == "derived") {
      derived = true;
    } else if (opt == "all") {
      base = true;
      derived = true;
    } else {
      out << " - wrong argument\n";
      return;
    }

    Vector< std::string > products = recipes.vertexIds();
    for (size_t i = 0; i < products.getSize(); ++i) {
      std::string product = products[i];
      if ((base && !(recipes.indegree(product))) || (derived && recipes.indegree(product))) {
        out << " - " << product << '\n';
      }
    }
  }

  void handleRecipe_add(std::istream &in, std::ostream &out, Baskets &, Recipes &recipes)
  {
    std::string dish;
    size_t d_qty = 0;
    size_t i_qty = 0;
    in >> dish >> d_qty >> i_qty;
    if (in.eof()) {
      return;
    }
    if (in.fail() || d_qty == 0 || i_qty == 0) {
      out << " - invalid quantity (must be natural number)\n";
      return;
    }
    if (recipes.hasVertex(dish)) {
      out << " - this dish already have a recipe\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return;
    }

    Vector< std::pair< std::string, size_t > > ingredients(i_qty);
    for (size_t i = 0; i < i_qty; ++i) {
      std::string ingredient = "";
      size_t qty = 0;
      in >> ingredient >> qty;
      if (in.eof()) {
        return;
      }
      if (in.fail() || qty == 0) {
        out << " - invalid ingredient quantity (must be natural number)\n";
        return;
      }
      if (!(recipes.hasVertex(ingredient))) {
        out << " - at least one of ingredients doesn't exist\n";
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        return;
      }
      for (size_t k = 0; k < i; ++k) {
        if (ingredient == ingredients[k].first) {
          out << " - at least one of ingredients mentioned more then one time\n";
          in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
          return;
        }
      }
      ingredients.pushBack({ingredient, qty});
    }

    try {
      recipes.addVertex({dish, d_qty}, dish);
    } catch (...) {
    }

    size_t i = 0;
    try {
      for (; i < i_qty; ++i) {
        recipes.addEdge(ingredients[i].first, dish, ingredients[i].second);
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        recipes.removeEdge(ingredients[j].first, dish);
      }
      recipes.removeVertex(dish);
      throw;
    }
  }

  void handleRecipe_remove(std::istream &in, std::ostream &out, Baskets &, Recipes &recipes)
  {
    std::string dish;
    in >> dish;
    if (in.eof()) {
      return;
    }

    if (recipes.hasVertex(dish)) {
      if (recipes.outdegree(dish)) {
        out << " - cannot remove this recipe: this dish is used as an ingredient\n";
        return;
      }
      recipes.removeVertex(dish);
      return;
    }
    out << " - no such dish\n";
    return;
  }

  void handleShow(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &)
  {
    std::string name;
    in >> name;
    if (in.eof()) {
      return;
    }

    if (!(baskets.has(name))) {
      out << " - no such basket\n";
      return;
    }

    Basket *bskt = baskets.get(name);
    auto it = bskt->begin();
    if (it == bskt->end()) {
      out << " - this one is empty\n";
      return;
    }
    while (it != bskt->end()) {
      out << " - " << it->first << ": " << it->second << '\n';
      ++it;
    }
  }

  void handleAdd(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &recipes)
  {
    std::string b_name, item;
    size_t qty = 0;
    in >> b_name >> item >> qty;
    if (in.eof()) {
      return;
    }
    if (in.fail() || qty == 0) {
      out << " - invalid quantity (must be natural number)\n";
      return;
    }

    if (!(baskets.has(b_name))) {
      out << " - no such basket\n";
      return;
    }
    if (!(recipes.hasVertex(item))) {
      out << " - no such ingredient/dish\n";
      return;
    }

    Basket *basket = baskets.get(b_name);
    if (basket->has(item)) {
      basket->at(item) += qty;
    } else {
      basket->add(item, qty);
    }
  }

  void handleRemove(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &recipes)
  {
    std::string b_name, item;
    size_t qty = 0;
    in >> b_name >> item >> qty;
    if (in.eof()) {
      return;
    }
    if (in.fail() || qty == 0) {
      out << " - invalid quantity (must be natural number)\n";
      return;
    }

    if (!(baskets.has(b_name))) {
      out << " - no such basket\n";
      return;
    }
    if (!(recipes.hasVertex(item))) {
      out << " - no such ingredient/dish in the base\n";
      return;
    }

    Basket *basket = baskets.get(b_name);
    if (!(basket->has(item))) {
      out << " - no such ingredient/dish in this busket\n";
      return;
    }
    if (basket->at(item) <= qty) {
      basket->remove(item);
    } else {
      basket->at(item) -= qty;
    }
  }

  void handleCook(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &recipes)
  {
    std::string b_name, dish;
    size_t asked_qty = 0;
    in >> b_name >> dish >> asked_qty;
    if (in.eof()) {
      return;
    }
    if (in.fail() || asked_qty == 0) {
      out << " - invalid quantity (must be natural number)\n";
      return;
    }

    if (!(baskets.has(b_name))) {
      out << " - no such basket\n";
      return;
    }
    if (!(recipes.hasVertex(dish))) {
      out << " - no such dish in the base\n";
      return;
    }

    Basket *basket = baskets.get(b_name);

    size_t vertex_qty = recipes.getVertexData(dish).second;
    size_t rCycles = (asked_qty + vertex_qty - 1) / vertex_qty;
    Vector< std::string > ingredients = recipes.incomingEdges(dish);
    size_t pCycles = rCycles;
    for (size_t i = 0; i < ingredients.getSize(); ++i) {
      std::string ingredient = ingredients[i];
      if (!(basket->has(ingredient))) {
        pCycles = 0;
        break;
      }
      size_t n = basket->get(ingredient) / recipes.getEdgeData(ingredient, dish);
      pCycles = (n < pCycles) ? n : pCycles;

      if (pCycles == 0) {
        break;
      }
    }

    size_t cycles = (pCycles >= rCycles) ? rCycles : pCycles;
    for (size_t i = 0; i < ingredients.getSize(); ++i) {
      std::string ingredient = ingredients[i];
      size_t cost = cycles * recipes.getEdgeData(ingredient, dish);
      if (basket->at(ingredient) == cost) {
        basket->remove(ingredient);
      } else {
        basket->at(ingredient) -= cost;
      }
    }
    if (basket->has(dish)) {
      basket->at(dish) += cycles * vertex_qty;
    } else {
      basket->add(dish, cycles * vertex_qty);
    }

    out << " - cooked " << cycles * vertex_qty << ' ' << dish << '\n';
  }

  void handleCookable(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &recipes)
  {
    std::string b_name, opt;
    in >> b_name >> opt;
    if (in.eof()) {
      return;
    }

    if (!(baskets.has(b_name))) {
      out << " - no such basket\n";
      return;
    }

    bool mode = true;
    if (opt == "direct") {
      mode = true;
    } else if (opt == "full") {
      mode = false;
    } else {
      out << " - wrong argument\n";
      return;
    }

    Basket *basket = baskets.get(b_name);

    Vector< std::string > vers = recipes.vertexIds();
    bool any = false;
    for (size_t i = 0; i < vers.getSize(); ++i) {
      std::string dish = vers[i];
      if (recipes.indegree(dish) == 0) {
        continue;
      }

      Vector< std::string > ingredients = recipes.incomingEdges(dish);
      Basket required(ingredients.getSize(), std::hash< std::string >(), std::equal_to< std::string >());
      for (size_t j = 0; j < ingredients.getSize(); ++j) {
        required.add(ingredients[j], recipes.getEdgeData(ingredients[j], dish));
      }

      bool flag = true;
      bool success = true;
      while (flag) {
        flag = false;
        auto it = required.begin();
        while (it != required.end()) {
          std::string ingredient = it->first;
          size_t proportion = it->second;

          if (!(basket->has(ingredient))) {
            if (mode || recipes.indegree(ingredient) == 0) {
              success = false;
              break;
            }
            ++it;
            required.remove(ingredient);

            Vector< std::string > newIngredients = recipes.incomingEdges(ingredient);
            for (size_t k = 0; k < newIngredients.getSize(); ++k) {
              if (required.has(newIngredients[k])) {
                required.at(newIngredients[k]) += proportion * recipes.getEdgeData(newIngredients[k], ingredient);
              } else {
                required.add(newIngredients[k], proportion * recipes.getEdgeData(newIngredients[k], ingredient));
              }
            }
            flag = true;
            continue;
          }
          if (basket->get(ingredient) < proportion) {
            if (mode || recipes.indegree(ingredient) == 0) {
              success = false;
              break;
            }
            required.at(ingredient) = basket->get(ingredient);

            proportion -= basket->get(ingredient);
            Vector< std::string > newIngredients = recipes.incomingEdges(ingredient);
            for (size_t k = 0; k < newIngredients.getSize(); ++k) {
              if (required.has(newIngredients[k])) {
                required.at(newIngredients[k]) += proportion * recipes.getEdgeData(newIngredients[k], ingredient);
              } else {
                required.add(newIngredients[k], proportion * recipes.getEdgeData(newIngredients[k], ingredient));
              }
            }
            flag = true;
          }
          ++it;
        }
      }
      if (success) {
        out << " - " << dish << '\n';
        any = true;
      }
    }
    if (!any) {
      out << " - no cookable dishes\n";
    }
  }

  void handleShow_recipe(std::istream &in, std::ostream &out, Baskets &baskets, Recipes &recipes)
  {}

}

#endif
