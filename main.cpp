#include <iostream>
#include <fstream>
#include <limits>
#include "library.hpp"
#include "config.hpp"
#include "menu.hpp"

const std::string DB_FILE   = "library.db";
const std::string CONF_FILE = "app.conf";

int main() {
    // ── Chargement de la configuration ──────────────────────────────
    Config config = loadConfig(CONF_FILE);

    // Si app.conf n'existe pas, on le crée avec les valeurs par défaut
    {
        std::ifstream f(CONF_FILE);
        if (!f.is_open()) saveConfig(config, CONF_FILE);
    }

    // ── Chargement ou création de la bibliothèque ────────────────────
    Library lib;
    {
        std::ifstream f(DB_FILE);
        if (f.is_open()) {
            f.close();
            lib = loadLibrary(DB_FILE);
        } else {
            // Première utilisation : création de la bibliothèque
            std::cout << config.logo << "\n";
            std::cout << "Bienvenue ! Aucune bibliothèque trouvée.\n";
            std::cout << "Créons-en une nouvelle.\n\n";
            std::cout << "Titre de la bibliothèque : ";
            std::getline(std::cin, lib.title);
            std::cout << "Description (ligne vide pour terminer) :\n";
            std::string line;
            while (std::getline(std::cin, line) && !line.empty()) {
                if (!lib.description.empty()) lib.description += "\n";
                lib.description += line;
            }
            saveLibrary(lib, DB_FILE);
            std::cout << "\n✓ Bibliothèque créée !\n";
            std::cout << "Appuyez sur Entrée pour continuer...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // ── Menu principal ───────────────────────────────────────────────
    while (true) {
        std::cout << "\033[2J\033[H"; // effacer l'écran
        std::cout << config.logo << "\n";
        std::cout << "══════════════════════════════════════════\n";
        std::cout << "  MENU PRINCIPAL  —  " << lib.title << "\n";
        std::cout << "══════════════════════════════════════════\n\n";
        std::cout << "  1. Consulter les références  (" << lib.books.size() << " livre(s))\n";
        std::cout << "  2. Gérer les références\n";
        std::cout << "  3. Chercher une référence\n";
        std::cout << "  4. Exporter en HTML\n";
        std::cout << "  5. Paramètres\n";
        std::cout << "  6. Quitter\n\n";

        int choice;
        std::cout << "> ";
        while (!(std::cin >> choice) || choice < 1 || choice > 6) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Choix invalide (1-6) : ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                menuConsulter(lib, config);
                break;
            case 2:
                menuGerer(lib, config, DB_FILE);
                break;
            case 3:
                menuChercher(lib, config);
                break;
            case 4: {
                std::string htmlOut = "bibliotheque.html";
                exportHTML(lib, htmlOut);
                std::cout << "\n✓ Export HTML généré : " << htmlOut << "\n";
                std::cout << "Appuyez sur Entrée...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 5:
                menuParametres(lib, config, DB_FILE, CONF_FILE);
                break;
            case 6:
                std::cout << "\nAu revoir !\n";
                return 0;
        }
    }
}
