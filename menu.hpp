#ifndef MENU_HPP
#define MENU_HPP

#include "library.hpp"
#include "config.hpp"

// Affiche le logo + un titre de menu
void printHeader(const Config& config, const std::string& menuName);

// Affiche une liste paginée de livres et retourne l'index choisi (-1 = retour)
int showPagedList(const std::vector<int>& indices, const Library& lib,
                  const Config& config);

// Affiche le détail complet d'un livre
void showBookDetail(const Book& book);

//  Menus principaux 

void menuConsulter(Library& lib, Config& config);
void menuGerer(Library& lib, Config& config, const std::string& dbFile);
void menuChercher(Library& lib, Config& config);
void menuParametres(Library& lib, Config& config,
                    const std::string& dbFile, const std::string& confFile);

// Saisie d'un nouveau livre
Book saisirLivre();

#endif
