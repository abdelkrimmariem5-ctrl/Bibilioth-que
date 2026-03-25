#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include "book.hpp"
#include <string>
#include <vector>

// Représente la bibliothèque complète
struct Library {
    std::string title;       // Titre de la bibliothèque
    std::string description; // Description
    std::vector<Book> books; // Liste des livres
};

// Charge la bibliothèque depuis library.db
Library loadLibrary(const std::string& filename);

// Sauvegarde la bibliothèque dans library.db
void saveLibrary(const Library& lib, const std::string& filename);

// Ajoute un livre à la bibliothèque
void addBook(Library& lib, const Book& book);

// Supprime tous les livres
void clearBooks(Library& lib);

// Importe des livres depuis un fichier CSV
int importCSV(Library& lib, const std::string& csvFile);

// Exporte la bibliothèque en HTML
void exportHTML(const Library& lib, const std::string& htmlFile);

// Recherche par ISBN exact
int findByISBN(const Library& lib, const std::string& isbn);

// Recherche par code éditeur
std::vector<int> findByPublisher(const Library& lib, const std::string& pubCode);

// Recherche par titre (insensible à la casse)
std::vector<int> findByTitle(const Library& lib, const std::string& query);

#endif
