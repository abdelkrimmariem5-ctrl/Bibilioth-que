#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>

// Structure représentant un livre
struct Book {
    std::string isbn;        // ISBN-13 (ex: 978-2-07-289509-8)
    std::string title;       // Titre
    std::string authors;     // Auteurs (séparés par virgules)
    std::string language;    // Langue
    std::string date;        // Date de parution
    std::string genre;       // Genre
    std::string description; // Description

    // Constructeur par défaut
    Book() {}

    // Constructeur complet
    Book(std::string isbn, std::string title, std::string authors,
         std::string language, std::string date, std::string genre,
         std::string description)
        : isbn(isbn), title(title), authors(authors),
          language(language), date(date), genre(genre),
          description(description) {}
};

// Valide le format et le chiffre de contrôle d'un ISBN-13
bool isValidISBN(const std::string& isbn);

// Extrait le code éditeur depuis un ISBN-13
std::string getPublisherCode(const std::string& isbn);

#endif
