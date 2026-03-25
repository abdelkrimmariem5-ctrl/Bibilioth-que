#include "menu.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>

// ── Utilitaires d'affichage ──────────────────────────────────────────

void printHeader(const Config& config, const std::string& menuName) {
    // Effacer l'écran
    std::cout << "\033[2J\033[H";
    std::cout << config.logo << "\n";
    std::cout << "══════════════════════════════════════════\n";
    std::cout << "  " << menuName << "\n";
    std::cout << "══════════════════════════════════════════\n\n";
}

// Lit un entier dans la plage [min, max]
static int readInt(int min, int max) {
    int choice;
    while (true) {
        std::cout << "> ";
        if (std::cin >> choice && choice >= min && choice <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Choix invalide. Entrez un nombre entre "
                  << min << " et " << max << " : ";
    }
}

// Lit une ligne non vide
static std::string readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin, line);
    return line;
}

// Lit plusieurs lignes jusqu'à une ligne vide
static std::string readMultiLine(const std::string& prompt) {
    std::cout << prompt << " (ligne vide pour terminer)\n";
    std::string result, line;
    while (std::getline(std::cin, line) && !line.empty()) {
        if (!result.empty()) result += "\n";
        result += line;
    }
    return result;
}

// ── Liste paginée ────────────────────────────────────────────────────

int showPagedList(const std::vector<int>& indices, const Library& lib,
                  const Config& config) {
    if (indices.empty()) {
        std::cout << "  Aucun résultat.\n";
        std::cout << "\nAppuyez sur Entrée...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }

    int total = (int)indices.size();
    int perPage = config.itemsPerPage;
    int page = 0;
    int totalPages = (total + perPage - 1) / perPage;

    while (true) {
        std::cout << "\033[2J\033[H";
        std::cout << "  Liste (" << total << " livre(s)) — Page "
                  << page + 1 << "/" << totalPages << "\n";
        std::cout << "──────────────────────────────────────────\n";

        int start = page * perPage;
        int end = std::min(start + perPage, total);
        for (int i = start; i < end; i++) {
            const Book& b = lib.books[indices[i]];
            std::cout << "  " << (i - start + 1) << ". [" << b.isbn << "]\n";
            std::cout << "     " << b.title << "\n";
            std::cout << "     Auteurs : " << b.authors << "\n\n";
        }

        std::cout << "──────────────────────────────────────────\n";
        std::cout << "  [1-" << (end - start) << "] Voir détail\n";
        if (page > 0)          std::cout << "  [p] Page précédente\n";
        if (page < totalPages - 1) std::cout << "  [s] Page suivante\n";
        std::cout << "  [0] Retour\n> ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "0") return -1;
        if (input == "s" || input == "S") {
            if (page < totalPages - 1) page++;
        } else if (input == "p" || input == "P") {
            if (page > 0) page--;
        } else {
            try {
                int n = std::stoi(input);
                if (n >= 1 && n <= (end - start)) {
                    return indices[start + n - 1];
                }
            } catch (...) {}
        }
    }
}

// ── Détail d'un livre ────────────────────────────────────────────────

void showBookDetail(const Book& book) {
    std::cout << "\n══════════════════════════════════════════\n";
    std::cout << "  DÉTAIL DU LIVRE\n";
    std::cout << "══════════════════════════════════════════\n";
    std::cout << "  Titre       : " << book.title       << "\n";
    std::cout << "  Auteurs     : " << book.authors     << "\n";
    std::cout << "  ISBN        : " << book.isbn        << "\n";
    std::cout << "  Langue      : " << book.language    << "\n";
    std::cout << "  Parution    : " << book.date        << "\n";
    std::cout << "  Genre       : " << book.genre       << "\n";
    std::cout << "  Description :\n    " << book.description << "\n";
    std::cout << "══════════════════════════════════════════\n";
    std::cout << "\nAppuyez sur Entrée pour revenir...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ── Saisie d'un livre ────────────────────────────────────────────────

Book saisirLivre() {
    Book b;
    std::cout << "\n── Ajout d'un nouveau livre ──\n";

    // ISBN avec validation
    while (true) {
        b.isbn = readLine("ISBN-13 (ex: 978-2-07-289509-8) : ");
        if (isValidISBN(b.isbn)) break;
        std::cout << "ISBN invalide. Vérifiez le format et le chiffre de contrôle.\n";
    }

    b.title       = readLine("Titre       : ");
    b.authors     = readLine("Auteurs     : ");
    b.language    = readLine("Langue      : ");
    b.date        = readLine("Date parution : ");
    b.genre       = readLine("Genre       : ");
    b.description = readMultiLine("Description");
    return b;
}

// ── Menu Consulter (F01) ─────────────────────────────────────────────

void menuConsulter(Library& lib, Config& config) {
    printHeader(config, "Consulter les références");

    std::vector<int> all(lib.books.size());
    for (int i = 0; i < (int)lib.books.size(); i++) all[i] = i;

    int idx = showPagedList(all, lib, config);
    if (idx >= 0) showBookDetail(lib.books[idx]);
}

// ── Menu Gérer (F02) ─────────────────────────────────────────────────

void menuGerer(Library& lib, Config& config, const std::string& dbFile) {
    while (true) {
        printHeader(config, "Gérer les références");
        std::cout << "  1. Ajouter une référence\n";
        std::cout << "  2. Importer des références (CSV)\n";
        std::cout << "  3. Supprimer toutes les références\n";
        std::cout << "  0. Retour\n";

        int choice = readInt(0, 3);
        if (choice == 0) return;

        if (choice == 1) {
            Book b = saisirLivre();
            addBook(lib, b);
            saveLibrary(lib, dbFile);
            std::cout << "\n✓ Livre ajouté avec succès !\n";
            std::cout << "Appuyez sur Entrée...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (choice == 2) {
            std::string csvFile = readLine("Nom du fichier CSV : ");
            int n = importCSV(lib, csvFile);
            if (n < 0) {
                std::cout << "Erreur : fichier introuvable.\n";
            } else {
                saveLibrary(lib, dbFile);
                std::cout << "✓ " << n << " livre(s) importé(s).\n";
            }
            std::cout << "Appuyez sur Entrée...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (choice == 3) {
            std::cout << "Confirmer la suppression de tous les livres ? (o/n) : ";
            std::string rep;
            std::getline(std::cin, rep);
            if (rep == "o" || rep == "O") {
                clearBooks(lib);
                saveLibrary(lib, dbFile);
                std::cout << "✓ Tous les livres ont été supprimés.\n";
                std::cout << "Appuyez sur Entrée...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

// ── Menu Chercher (F03) ──────────────────────────────────────────────

void menuChercher(Library& lib, Config& config) {
    while (true) {
        printHeader(config, "Chercher une référence");
        std::cout << "  1. Rechercher par ISBN-13\n";
        std::cout << "  2. Rechercher par code éditeur\n";
        std::cout << "  3. Rechercher par titre\n";
        std::cout << "  0. Retour\n";

        int choice = readInt(0, 3);
        if (choice == 0) return;

        if (choice == 1) {
            std::string isbn = readLine("ISBN-13 : ");
            int idx = findByISBN(lib, isbn);
            if (idx < 0) std::cout << "Aucun livre trouvé.\n";
            else showBookDetail(lib.books[idx]);
        }
        else if (choice == 2) {
            std::string pub = readLine("Code éditeur : ");
            std::vector<int> res = findByPublisher(lib, pub);
            int idx = showPagedList(res, lib, config);
            if (idx >= 0) showBookDetail(lib.books[idx]);
        }
        else if (choice == 3) {
            std::string query = readLine("Titre (ou partie) : ");
            std::vector<int> res = findByTitle(lib, query);
            int idx = showPagedList(res, lib, config);
            if (idx >= 0) showBookDetail(lib.books[idx]);
        }
    }
}

// ── Menu Paramètres (F05) ────────────────────────────────────────────

void menuParametres(Library& lib, Config& config,
                    const std::string& dbFile, const std::string& confFile) {
    while (true) {
        printHeader(config, "Paramètres");
        std::cout << "  1. Modifier le titre et la description de la bibliothèque\n";
        std::cout << "  2. Paramétrer l'affichage (livres par page)\n";
        std::cout << "  3. Modifier le logo ASCII\n";
        std::cout << "  0. Retour\n";

        int choice = readInt(0, 3);
        if (choice == 0) return;

        if (choice == 1) {
            std::cout << "Titre actuel : " << lib.title << "\n";
            std::string t = readLine("Nouveau titre (vide = conserver) : ");
            if (!t.empty()) lib.title = t;

            std::cout << "Description actuelle :\n" << lib.description << "\n";
            std::string d = readMultiLine("Nouvelle description (vide = conserver)");
            if (!d.empty()) lib.description = d;

            saveLibrary(lib, dbFile);
            std::cout << "✓ Informations mises à jour.\n";
            std::cout << "Appuyez sur Entrée...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (choice == 2) {
            std::cout << "Livres par page actuel : " << config.itemsPerPage << "\n";
            std::cout << "Nouveau nombre (1-50) : ";
            int n = readInt(1, 50);
            config.itemsPerPage = n;
            saveConfig(config, confFile);
            std::cout << "✓ Paramètre sauvegardé.\n";
            std::cout << "Appuyez sur Entrée...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (choice == 3) {
            std::cout << "Logo actuel :\n" << config.logo << "\n";
            std::string logo = readMultiLine("Nouveau logo ASCII (vide = conserver)");
            if (!logo.empty()) config.logo = logo + "\n";
            saveConfig(config, confFile);
            std::cout << "✓ Logo mis à jour.\n";
            std::cout << "Appuyez sur Entrée...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}
