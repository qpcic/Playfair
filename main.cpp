#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

// Generiramo 16x16 matriko (256 znakov)
std::vector<unsigned char> generateMatrix(const std::string& key) {
    std::unordered_set<unsigned char> seen;
    std::vector<unsigned char> matrix;

    // 1. Dodamo unikatne znake iz ključa
    for (unsigned char ch : key) {
        if (seen.find(ch) == seen.end()) {
            matrix.push_back(ch);
            seen.insert(ch);
        }
    }

    // 2. Dodamo ostale ASCII znake (0–255), ki še niso vključeni
    for (int i = 0; i < 256; ++i) {
        unsigned char ch = static_cast<unsigned char>(i);
        if (seen.find(ch) == seen.end()) {
            matrix.push_back(ch);
            seen.insert(ch);
        }
    }

    return matrix; // 256 znakov
}

// Izpis matrike v 16x16 formatu
void printMatrix(const std::vector<unsigned char>& matrix) {
    for (int i = 0; i < 256; ++i) {
        if (i % 16 == 0 && i != 0)
            std::cout << "\n";
        std::cout << std::hex << (int)matrix[i] << " ";
    }
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uporaba: " << argv[0] << " <e ali d> <ključ> <datoteka>\n";
        return 1;
    }

    std::string key = argv[2];

    std::vector<unsigned char> matrix = generateMatrix(key);

    std::cout << "Ustvarjena matrika (256 znakov):\n";
    printMatrix(matrix);

    return 0;
}
