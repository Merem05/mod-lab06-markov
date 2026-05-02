#include "textgen.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Размер префикса: " << NPREF << " слова\n";
    cout << "Макс объем текста: " << MAXGEN << " слов\n\n";

    statetab table;
    prefix startPrefix;

    string filename = "input.txt";

    buildTable(filename, table, startPrefix);

    if (table.empty())
    {
        cerr << "Ошибка при загрузке файла\n";
        return 1;
    }

    cout << "Файл успешно загружен\n";

    string generatedText = generate(table, startPrefix, MAXGEN);

    if (generatedText.empty())
    {
        cerr << "Ошибка при генерации текста.\n";
        return 1;
    }

    cout << "\n" << generatedText << "\n";

    saveToFile("gen.txt", generatedText);

    return 0;
}