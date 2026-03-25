#include "config.hpp"
#include <fstream>
#include <sstream>

// Charge la configuration depuis le fichier
Config loadConfig(const std::string& filename) {
    Config config;
    std::ifstream file(filename);
    if (!file.is_open()) return config; // valeurs par défaut

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("items_per_page=", 0) == 0) {
            config.itemsPerPage = std::stoi(line.substr(15));
        } else if (line == "[logo]") {
            // Lire le logo (lignes jusqu'à [end_logo])
            std::string logoLine;
            config.logo = "";
            while (std::getline(file, logoLine)) {
                if (logoLine == "[end_logo]") break;
                config.logo += logoLine + "\n";
            }
        }
    }
    return config;
}

// Sauvegarde la configuration dans le fichier
void saveConfig(const Config& config, const std::string& filename) {
    std::ofstream file(filename);
    file << "items_per_page=" << config.itemsPerPage << "\n";
    file << "[logo]\n" << config.logo << "[end_logo]\n";
}
