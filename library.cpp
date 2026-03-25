#include "library.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

// ── Lecture/écriture fichier ─────────────────────────────────────────

Library loadLibrary(const std::string& filename) {
    Library lib;
    std::ifstream file(filename);
    if (!file.is_open()) return lib;

    std::string line;
    // Titre
    if (std::getline(file, line)) lib.title = line;
    // Description (jusqu'à [books])
    lib.description = "";
    while (std::getline(file, line)) {
        if (line == "[books]") break;
        if (!lib.description.empty()) lib.description += "\n";
        lib.description += line;
    }
    // Livres
    while (std::getline(file, line)) {
        if (line == "[book]") {
            Book b;
            std::getline(file, b.isbn);
            std::getline(file, b.title);
            std::getline(file, b.authors);
            std::getline(file, b.language);
            std::getline(file, b.date);
            std::getline(file, b.genre);
            // Description du livre (jusqu'à [end_book])
            b.description = "";
            std::string descLine;
            while (std::getline(file, descLine)) {
                if (descLine == "[end_book]") break;
                if (!b.description.empty()) b.description += "\n";
                b.description += descLine;
            }
            lib.books.push_back(b);
        }
    }
    return lib;
}

void saveLibrary(const Library& lib, const std::string& filename) {
    std::ofstream file(filename);
    file << lib.title << "\n";
    file << lib.description << "\n";
    file << "[books]\n";
    for (const Book& b : lib.books) {
        file << "[book]\n";
        file << b.isbn << "\n";
        file << b.title << "\n";
        file << b.authors << "\n";
        file << b.language << "\n";
        file << b.date << "\n";
        file << b.genre << "\n";
        file << b.description << "\n";
        file << "[end_book]\n";
    }
}

// ── Gestion des livres ───────────────────────────────────────────────

void addBook(Library& lib, const Book& book) {
    lib.books.push_back(book);
}

void clearBooks(Library& lib) {
    lib.books.clear();
}

// ── Import CSV ───────────────────────────────────────────────────────
// Format CSV attendu : isbn,titre,auteurs,langue,date,genre,description

int importCSV(Library& lib, const std::string& csvFile) {
    std::ifstream file(csvFile);
    if (!file.is_open()) return -1;

    int count = 0;
    std::string line;
    std::getline(file, line); // ignorer l'en-tête

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string fields[7];
        for (int i = 0; i < 7; i++) {
            std::getline(ss, fields[i], ',');
        }
        Book b(fields[0], fields[1], fields[2],
               fields[3], fields[4], fields[5], fields[6]);
        lib.books.push_back(b);
        count++;
    }
    return count;
}

// ── Export HTML ──────────────────────────────────────────────────────

// Retire les articles courants pour le tri alphabétique
static std::string sortKey(const std::string& title) {
    std::vector<std::string> articles = {
        "le ", "la ", "les ", "l'", "un ", "une ",
        "des ", "j'", "d'", "qu'", "s'", "c'"
    };
    std::string low = title;
    std::transform(low.begin(), low.end(), low.begin(), ::tolower);
    for (const std::string& art : articles) {
        if (low.rfind(art, 0) == 0) {
            return low.substr(art.size());
        }
    }
    return low;
}

void exportHTML(const Library& lib, const std::string& htmlFile) {
    // Trier les livres par clé de tri
    std::vector<int> indices(lib.books.size());
    for (int i = 0; i < (int)lib.books.size(); i++) indices[i] = i;
    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        return sortKey(lib.books[a].title) < sortKey(lib.books[b].title);
    });

    // Collecter les lettres présentes
    std::string presentLetters = "";
    for (int i : indices) {
        std::string key = sortKey(lib.books[i].title);
        char c = key.empty() ? '#' : (isalpha(key[0]) ? toupper(key[0]) : '#');
        if (presentLetters.find(c) == std::string::npos)
            presentLetters += c;
    }

    std::ofstream file(htmlFile);
    file << "<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n"
         << "<meta charset=\"UTF-8\">\n"
         << "<title>" << lib.title << "</title>\n"
         << "<style>\n"
         << "body{font-family:sans-serif;max-width:900px;margin:0 auto;padding:20px;}\n"
         << "nav{position:sticky;top:0;background:#fff;padding:10px 0;border-bottom:2px solid #333;letter-spacing:5px;font-size:1.2rem;}\n"
         << "nav a{color:#c0396e;text-decoration:none;margin:0 4px;}\n"
         << "nav span{color:#aaa;margin:0 4px;}\n"
         << "h2{border-bottom:1px solid #ddd;padding-bottom:6px;color:#333;}\n"
         << ".book{margin:12px 0;padding:12px;border:1px solid #eee;border-radius:6px;}\n"
         << ".book h3{margin:0 0 6px;color:#c0396e;}\n"
         << ".book p{margin:3px 0;font-size:0.9rem;color:#555;}\n"
         << "</style>\n</head>\n<body>\n";

    file << "<h1>" << lib.title << "</h1>\n";
    file << "<p>" << lib.description << "</p>\n";

    // Index alphabétique
    file << "<nav>";
    std::string allLetters = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char c : allLetters) {
        if (presentLetters.find(c) != std::string::npos) {
            file << "<a href=\"#sec-" << c << "\">" << c << "</a>";
        } else {
            file << "<span>" << c << "</span>";
        }
    }
    file << "</nav>\n";

    // Sections par lettre
    char currentLetter = 0;
    for (int i : indices) {
        const Book& b = lib.books[i];
        std::string key = sortKey(b.title);
        char letter = key.empty() ? '#' : (isalpha(key[0]) ? toupper(key[0]) : '#');

        if (letter != currentLetter) {
            if (currentLetter != 0) file << "</div>\n";
            currentLetter = letter;
            file << "<h2 id=\"sec-" << letter << "\">" << letter << "</h2>\n<div>\n";
        }

        file << "<div class=\"book\">\n";
        file << "<h3>" << b.title << "</h3>\n";
        file << "<p><strong>Auteurs :</strong> " << b.authors << "</p>\n";
        file << "<p><strong>ISBN :</strong> " << b.isbn << "</p>\n";
        file << "<p><strong>Langue :</strong> " << b.language << "</p>\n";
        file << "<p><strong>Date :</strong> " << b.date << "</p>\n";
        file << "<p><strong>Genre :</strong> " << b.genre << "</p>\n";
        file << "<p>" << b.description << "</p>\n";
        file << "</div>\n";
    }
    if (currentLetter != 0) file << "</div>\n";
    file << "</body>\n</html>\n";
}

// ── Recherches ───────────────────────────────────────────────────────

int findByISBN(const Library& lib, const std::string& isbn) {
    for (int i = 0; i < (int)lib.books.size(); i++) {
        if (lib.books[i].isbn == isbn) return i;
    }
    return -1;
}

std::vector<int> findByPublisher(const Library& lib, const std::string& pubCode) {
    std::vector<int> results;
    for (int i = 0; i < (int)lib.books.size(); i++) {
        if (getPublisherCode(lib.books[i].isbn) == pubCode)
            results.push_back(i);
    }
    return results;
}

std::vector<int> findByTitle(const Library& lib, const std::string& query) {
    std::vector<int> results;
    std::string qLow = query;
    std::transform(qLow.begin(), qLow.end(), qLow.begin(), ::tolower);
    for (int i = 0; i < (int)lib.books.size(); i++) {
        std::string tLow = lib.books[i].title;
        std::transform(tLow.begin(), tLow.end(), tLow.begin(), ::tolower);
        if (tLow.find(qLow) != std::string::npos)
            results.push_back(i);
    }
    return results;
}
