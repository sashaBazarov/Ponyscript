#include "lib.h"
#include "ponystring.h"
#include <fstream>

ponystring readFile(const ponystring& filename) {
    std::ifstream file(filename.str());
    ponystring content;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    }
    return content;
}

bool writeToFile(const ponystring& filename, const ponystring& content) {
    std::ofstream file(filename.str());
    if (file.is_open()) {
        file << content.str();
        file.close();
        return true;
    } else {
        return false;
    }
}
