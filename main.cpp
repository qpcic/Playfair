#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>

using ParZnakov = std::string;

// ============================
// Branje datoteke
// ============================
std::vector<unsigned char> preberiVhodnoDatoteko(const std::string& pot) {
    std::ifstream vhod(pot, std::ios::binary);
    std::vector<unsigned char> podatki;

    if (!vhod) {
        std::cerr << "Napaka pri odpiranju datoteke: " << pot << "\n";
        return podatki;
    }

    unsigned char znak;
    while (vhod.read(reinterpret_cast<char*>(&znak), 1)) {
        podatki.push_back(znak);
    }

    // Odstrani UTF-8 BOM, če obstaja
    if (podatki.size() >= 3 && podatki[0] == 0xEF && podatki[1] == 0xBB && podatki[2] == 0xBF) {
        podatki.erase(podatki.begin(), podatki.begin() + 3);
    }

    // Ročno odstrani presledke in bele znake
    std::vector<unsigned char> brezPresledkov;
    for (size_t i = 0; i < podatki.size(); ++i) {
        unsigned char c = podatki[i];
        if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
            brezPresledkov.push_back(c);
        }
    }

    return brezPresledkov;
}

// ============================
// Izpis surovega besedila
// ============================
void izpisiSuroviVhod(const std::vector<unsigned char>& vhod) {
    std::cout << "Surov vhod (" << vhod.size() << " znakov):\n";
    for (unsigned char znak : vhod) {
        std::cout << std::hex << (int)znak << " ";
    }
    std::cout << "\n\n";
}

// ============================
// Predobdelava – pari znakov
// ============================
std::vector<ParZnakov> predobdelajVhod(const std::vector<unsigned char>& vhod) {
    std::vector<ParZnakov> pari;
    size_t i = 0;

    while (i < vhod.size()) {
        char prvi = vhod[i];
        char drugi;

        if (i + 1 >= vhod.size()) {
            drugi = 3; // ASCII EOF
            i++;
        } else if (vhod[i] == vhod[i + 1]) {
            drugi = '\0'; // ASCII NULL
            i++;
        } else {
            drugi = vhod[i + 1];
            i += 2;
        }

        pari.push_back(std::string{ prvi, drugi });
    }

    return pari;
}

void izpisiPare(const std::vector<ParZnakov>& pari) {
    std::cout << "Predobdelani pari:\n";
    for (const auto& par : pari) {
        std::cout << par << " ";
    }
    std::cout << "\n\n";
}

// ============================
// Generiranje 16x16 matrike kljuca
// ============================
void generirajMatriko(const std::string& kljuc, unsigned char M[16][16]) {
    std::unordered_set<unsigned char> videno;
    std::vector<unsigned char> linearno;

    for (unsigned char znak : kljuc) {
        if (videno.insert(znak).second) {
            linearno.push_back(znak);
        }
    }

    for (int i = 0; i < 256; ++i) {
        unsigned char znak = static_cast<unsigned char>(i);
        if (videno.insert(znak).second) {
            linearno.push_back(znak);
        }
    }

    for (int i = 0; i < 256; ++i) {
        M[i / 16][i % 16] = linearno[i];
    }
}

void izpisiMatriko(unsigned char M[16][16]) {
    std::cout << "Matrika kljuca (16x16):\n";
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            unsigned char znak = M[i][j];

            // Če je natisljiv ASCII znak (32 do 126), ga izpišemo kot znak
            if (znak >= 32 && znak <= 126)
                std::cout << " " << znak << " ";
            else
                std::cout << " . ";  // nadomestek za nenatisljive znake
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// ============================
// sifriranje
// ============================
std::pair<int, int> poisciPozicijo(unsigned char znak, unsigned char M[16][16]) {
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            if (M[i][j] == znak)
                return { i, j };

    return { -1, -1 }; // ne bi smelo nastati
}

std::vector<ParZnakov> sifrirajPare(const std::vector<ParZnakov>& pari, unsigned char M[16][16]) {
    std::vector<ParZnakov> sifrirano;

    for (const auto& par : pari) {
        unsigned char a = par[0];
        unsigned char b = par[1];

        auto [i, j] = poisciPozicijo(a, M);
        auto [k, f] = poisciPozicijo(b, M);

        char novoA, novoB;

        if (i != k && j != f) {
            novoA = M[i][f];
            novoB = M[k][j];
        } else if (i == k) {
            novoA = M[i][(j + 1) % 16];
            novoB = M[k][(f + 1) % 16];
        } else if (j == f) {
            novoA = M[(i + 1) % 16][j];
            novoB = M[(k + 1) % 16][f];
        } else {
            novoA = a;
            novoB = b;
        }

        sifrirano.push_back(std::string{ novoA, novoB });
    }

    return sifrirano;
}

std::vector<ParZnakov> desifrirajPare(const std::vector<ParZnakov>& pari, unsigned char M[16][16]) {
    std::vector<ParZnakov> desifrirano;

    for (const auto& par : pari) {
        char a = par[0];
        char b = par[1];

        auto [i, j] = poisciPozicijo(a, M);
        auto [k, f] = poisciPozicijo(b, M);

        char novoA, novoB;

        if (i != k && j != f) {
            novoA = M[i][f];
            novoB = M[k][j];
        } else if (i == k) {
            novoA = M[i][(j + 15) % 16];  // -1 mod 16
            novoB = M[k][(f + 15) % 16];
        } else if (j == f) {
            novoA = M[(i + 15) % 16][j];  // -1 mod 16
            novoB = M[(k + 15) % 16][f];
        } else {
            novoA = a;
            novoB = b;
        }

        desifrirano.push_back(std::string{ novoA, novoB });
    }

    return desifrirano;
}

void izpisiDesifrirano(const std::vector<ParZnakov>& pari) {
    std::cout << "Desifrirani pari:\n";
    for (const auto& par : pari) {
        std::cout << par << " ";
    }
    std::cout << "\n\n";
}


void izpisiSifrirano(const std::vector<ParZnakov>& sifrirano) {
    std::cout << "sifrirani pari:\n";
    for (const auto& par : sifrirano) {
        std::cout << par << " ";
    }
    std::cout << "\n\n";
}

void zapisiVDatoteko(const std::string& imeDatoteke, const std::vector<ParZnakov>& podatki) {
    std::ofstream izhod(imeDatoteke, std::ios::binary);

    if (!izhod) {
        std::cerr << "Napaka pri odpiranju za pisanje: " << imeDatoteke << "\n";
        return;
    }

    for (const auto& par : podatki) {
        izhod.write(par.c_str(), par.size()); // par.size() bo 2
    }

    std::cout << "Zapisano v datoteko: " << imeDatoteke << "\n";
}


// ============================
// MAIN
// ============================
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uporaba: " << argv[0] << " <e ali d> <kljuc> <datoteka>\n";
        return 1;
    }

    std::string nacin = argv[1];     // "e" ali "d"
    std::string kljuc = argv[2];
    std::string potDoDatoteke = argv[3];

    auto suroviVhod = preberiVhodnoDatoteko(potDoDatoteke);
    izpisiSuroviVhod(suroviVhod);

    auto pari = predobdelajVhod(suroviVhod);
    izpisiPare(pari);

    unsigned char M[16][16];
    generirajMatriko(kljuc, M);
    izpisiMatriko(M);

    if (nacin == "e") {
        auto sifrirano = sifrirajPare(pari, M);
        izpisiSifrirano(sifrirano);
        zapisiVDatoteko("out_e.txt", sifrirano);
    }
    else if (nacin == "d") {
        auto desifrirano = desifrirajPare(pari, M);
        izpisiDesifrirano(desifrirano);
        zapisiVDatoteko("out_d.txt", desifrirano);
    }

    return 0;
}
