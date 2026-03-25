#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

// Paramètres de l'application
struct Config {
    int itemsPerPage;    // Nombre de livres par page
    std::string logo;    // Logo ASCII Art

    Config() : itemsPerPage(5), logo(
        "  ____  _ _     _ _       _   _\n"
        " | __ )(_) |__ | (_) ___ | |_| |__   ___  __ _ _   _  ___\n"
        " |  _ \\| | '_ \\| | |/ _ \\| __| '_ \\ / _ \\/ _` | | | |/ _ \\\n"
        " | |_) | | |_) | | | (_) | |_| | | |  __/ (_| | |_| |  __/\n"
        " |____/|_|_.__/|_|_|\\___/ \\__|_| |_|\\___|\\__, |\\__,_|\\___|\n"
        "                                             |_|\n"
    ) {}
};

// Charge la config depuis app.conf
Config loadConfig(const std::string& filename);

// Sauvegarde la config dans app.conf
void saveConfig(const Config& config, const std::string& filename);

#endif
