#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>

using CharPair = std::pair<unsigned char, unsigned char>;

// ============================
// Branje datoteke
// ============================
std::vector<unsigned char> readInputFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    std::vector<unsigned char> data;

    if (!in) {
        std::cerr << "Napaka pri odpiranju datoteke: " << path << "\n";
        return data;
    }

    unsigned char ch;
    while (in.read(reinterpret_cast<char*>(&ch), 1)) {
        data.push_back(ch);
    }

    return data;
}

// ============================
// Izpis surovega vhodnega besedila
// ============================
void printRawInput(const std::vector<unsigned char>& input) {
    std::cout << "Surov vhodni niz (" << input.size() << " znakov):\n";
    for (unsigned char ch : input) {
        std::cout << std::hex << (int)ch << " ";
    }
    std::cout << "\n\n";
}

// ============================
// Predobdelava – razbijanje na pare
// ============================
std::vector<CharPair> preprocessInput(const std::vector<unsigned char>& input) {
    std::vector<CharPair> pairs;
    size_t i = 0;

    while (i < input.size()) {
        unsigned char first = input[i];
        unsigned char second;

        if (i + 1 >= input.size()) {
            second = 3; // ASCII EOF
            i++;
        } else if (input[i] == input[i + 1]) {
            second = '\0'; // NULL
            i += 1;
        } else {
            second = input[i + 1];
            i += 2;
        }

        pairs.emplace_back(first, second);
    }

    return pairs;
}

void printPairs(const std::vector<CharPair>& pairs) {
    std::cout << "Predobdelani pari znakov:\n";
    for (const auto& [a, b] : pairs) {
        std::cout << "(" << std::hex << (int)a << "," << (int)b << ") ";
    }
    std::cout << "\n\n";
}

// ============================
// Generiranje 16x16 matrike
// ============================
std::vector<unsigned char> generateMatrix(const std::string& key) {
    std::unordered_set<unsigned char> seen;
    std::vector<unsigned char> matrix;

    for (unsigned char ch : key) {
        if (seen.find(ch) == seen.end()) {
            matrix.push_back(ch);
            seen.insert(ch);
        }
    }

    for (int i = 0; i < 256; ++i) {
        unsigned char ch = static_cast<unsigned char>(i);
        if (seen.find(ch) == seen.end()) {
            matrix.push_back(ch);
            seen.insert(ch);
        }
    }

    return matrix;
}

void printMatrix(const std::vector<unsigned char>& matrix) {
    std::cout << "Matrika ključa (16x16):\n";
    for (int i = 0; i < 256; ++i) {
        if (i % 16 == 0 && i != 0)
            std::cout << "\n";
        std::cout << std::hex << (int)matrix[i] << " ";
    }
    std::cout << "\n\n";
}

// ============================
// MAIN
// ============================
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uporaba: " << argv[0] << " <e ali d> <ključ> <datoteka>\n";
        return 1;
    }

    std::string mode = argv[1];     // "e" ali "d"
    std::string key = argv[2];
    std::string filepath = argv[3];

    auto rawInput = readInputFile(filepath);
    printRawInput(rawInput);

    auto pairs = preprocessInput(rawInput);
    printPairs(pairs);

    auto matrix = generateMatrix(key);
    printMatrix(matrix);

    // TODO: Tukaj bo kasneje dodan algoritem za šifriranje/dešifriranje

    return 0;
}
