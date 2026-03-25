#include "book.hpp"
#include <sstream>
#include <vector>

// Valide un ISBN-13 : format et chiffre de contrôle
bool isValidISBN(const std::string& isbn) {
    // Retirer les tirets
    std::string digits;
    for (char c : isbn) {
        if (c == '-') continue;
        if (!isdigit(c)) return false;
        digits += c;
    }

    if (digits.size() != 13) return false;

    // Calcul du chiffre de contrôle ISBN-13
    int sum = 0;
    for (int i = 0; i < 12; i++) {
        int d = digits[i] - '0';
        sum += (i % 2 == 0) ? d : d * 3;
    }
    int check = (10 - (sum % 10)) % 10;
    return check == (digits[12] - '0');
}

// Extrait le code éditeur (3ème partie de l'ISBN séparé par tirets)
std::string getPublisherCode(const std::string& isbn) {
    std::vector<std::string> parts;
    std::stringstream ss(isbn);
    std::string part;
    while (std::getline(ss, part, '-')) {
        parts.push_back(part);
    }
    if (parts.size() >= 3) return parts[2];
    return "";
}
