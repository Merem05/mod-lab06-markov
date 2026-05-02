#include "textgen.h"

unsigned int currentSeed = 0;

void setSeed(unsigned int seed)
{
    currentSeed = seed;
    srand(seed);
}

void buildTable(const string& filename, statetab& table, prefix& startPrefix)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return;
    }

    vector<string> words;
    string word;
    while (file >> word)
    {
        words.push_back(word);
    }
    file.close();

    if (words.size() < NPREF + 1)
    {
        cerr << "Ошибка: текст слишком короткий" << endl;
        return;
    }

    startPrefix.clear();
    for (int i = 0; i < NPREF; i++)
    {
        startPrefix.push_back(words[i]);
    }

    for (size_t i = 0; i < words.size() - NPREF; i++)
    {
        prefix current;
        for (int j = 0; j < NPREF; j++)
        {
            current.push_back(words[i + j]);
        }
        string suffix = words[i + NPREF];
        table[current].push_back(suffix);
    }
}

string generate(const statetab& table, const prefix& startPrefix, int maxGen)
{
    if (table.empty())
    {
        cerr << "Ошибка: таблица пуста" << endl;
        return "";
    }

    if (currentSeed == 0)
    {
        srand(time(NULL));
    }
    else
    {
        srand(currentSeed);
    }

    string W1 = startPrefix[0];
    string W2 = startPrefix[1];

    string result = W1 + " " + W2;

    for (int step = 0; step < maxGen; step++)
    {
        prefix key;
        key.push_back(W1);
        key.push_back(W2);

        auto it = table.find(key);
        if (it == table.end() || it->second.empty())
            break;

        const vector<string>& suffixes = it->second;
        int randomIndex = rand() % suffixes.size();
        string W3 = suffixes[randomIndex];

        result += " " + W3;

        W1 = W2;
        W2 = W3;
    }

    return result;
}

void saveToFile(const string& filename, const string& text)
{
    string dir = "result";
    system(("mkdir " + dir + " 2>nul").c_str());

    string fullPath = dir + "/" + filename;
    ofstream file(fullPath);
    if (file.is_open())
    {
        file << text;
        file.close();
        cout << "Текст сохранён в " << fullPath << endl;
    }
    else
    {
        cerr << "Ошибка: не удалось сохранить файл " << fullPath << endl;
    }
}