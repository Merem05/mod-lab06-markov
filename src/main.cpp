#include "textgen.h"

int main() {
    std::cout << "Prefix size: " << NPREF << " words\n";
    std::cout << "Max text size: " << MAXGEN << " words\n\n";

    statetab table;
    prefix startPrefix;

    string filename = "input.txt";
    std::cout << "Loading file: " << filename << "...\n";

    buildTable(filename, table, startPrefix);

    if (table.empty()) {
        std::cerr << "Error: cannot load file\n";
        return 1;
    }

    std::cout << "File loaded successfully\n";

    string generatedText = generate(table, startPrefix, MAXGEN);

    if (generatedText.empty()) {
        std::cerr << "Error: cannot generate text\n";
        return 1;
    }

    std::cout << "\n" << generatedText << "\n";

    saveToFile("gen.txt", generatedText);

    return 0;
}
