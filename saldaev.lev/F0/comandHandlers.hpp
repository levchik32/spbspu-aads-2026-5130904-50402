#ifndef COMANDHANDLERS_HPP
#define COMANDHANDLERS_HPP
#include <iostream>
#include "Graph.hpp"

namespace saldaev
{
  using Recipes = saldaev::Graph< std::pair< std::string, size_t >, size_t >;
  using Basket = saldaev::HashTable< std::string, size_t, std::hash< std::string >, std::equal_to< std::string > >;
  using Baskets = saldaev::HashTable< std::string, Basket *, std::hash< std::string >, std::equal_to< std::string > >;

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
    size_t size = sizeof(HELP_DATA) / sizeof(HELP_DATA[0]);
    for (size_t i = 0; i < size; ++i) {
      if (HELP_DATA[i].first == com) {
        out << HELP_DATA[i].second << '\n';
        return;
      }
    }
    out << "no such command\n";
  }
}

#endif
