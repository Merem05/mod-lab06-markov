#include <gtest/gtest.h>
#include "textgen.h"

using namespace std;

TEST(PrefixTest, ThreeWords)
{
    prefix p;
    p.push_back("город");
    p.push_back("деревня");
    p.push_back("страна");

    EXPECT_EQ(p.size(), 3);
    EXPECT_EQ(p[0], "город");
    EXPECT_EQ(p[1], "деревня");
    EXPECT_EQ(p[2], "страна");
}

TEST(PrefixTest, TwoWords)
{
    prefix p;
    p.push_back("белый");
    p.push_back("красный");

    EXPECT_EQ(p.size(), 2);
    EXPECT_EQ(p[0], "белый");
    EXPECT_EQ(p[1], "красный");
}

TEST(StateTabTest, PrefixSuffixRecord)
{
    statetab tab;
    prefix key;
    key.push_back("мама");
    key.push_back("готовила");

    tab[key].push_back("плов");
    tab[key].push_back("суп");

    EXPECT_EQ(tab[key].size(), 2);
    EXPECT_EQ(tab[key][0], "плов");
    EXPECT_EQ(tab[key][1], "суп");
}

TEST(StateTabTest, SingleSuffix)
{
    statetab tab;
    prefix key;
    key.push_back("синий");
    key.push_back("зеленый");
    tab[key].push_back("розовый");

    EXPECT_EQ(tab[key].size(), 1);
    EXPECT_EQ(tab[key][0], "розовый");
}

TEST(StateTabTest, MultipleSuffixes)
{
    statetab tab;
    prefix key;
    key.push_back("хлеб");
    key.push_back("масло");

    tab[key].push_back("сыр");
    tab[key].push_back("молоко");
    tab[key].push_back("сметана");

    EXPECT_EQ(tab[key].size(), 3);
}

TEST(GenerateTest, ManualTable)
{
    statetab tab;
    prefix start;

    start.push_back("1");
    start.push_back("2");

    prefix k1;
    k1.push_back("1");
    k1.push_back("2");
    tab[k1].push_back("3");

    prefix k2;
    k2.push_back("2");
    k2.push_back("3");
    tab[k2].push_back("4");

    setSeed(42);
    string res = generate(tab, start, 2);

    EXPECT_NE(res.find("1"), string::npos);
    EXPECT_NE(res.find("2"), string::npos);
}

TEST(GenerateTest, ZeroLength)
{
    statetab tab;
    prefix start;

    start.push_back("а");
    start.push_back("б");

    prefix k;
    k.push_back("а");
    k.push_back("б");
    tab[k].push_back("в");

    setSeed(1);
    string res = generate(tab, start, 0);

    int words = 1;
    for (char c : res) if (c == ' ') words++;

    EXPECT_EQ(words, 2);
}

TEST(GenerateTest, EmptyTable)
{
    statetab pusto;
    prefix start;

    string res = generate(pusto, start, 10);

    EXPECT_TRUE(res.empty());
}

TEST(GenerateTest, RandomSuffixSelection)
{
    statetab tab;
    prefix start;
    start.push_back("1");
    start.push_back("2");

    prefix k;
    k.push_back("1");
    k.push_back("2");
    tab[k].push_back("3");
    tab[k].push_back("4");
    tab[k].push_back("5");

    bool saw3 = false;
    bool saw4 = false;
    bool saw5 = false;

    for (int i = 1; i <= 20; i++)
    {
        setSeed(i);
        string res = generate(tab, start, 1);
        if (res.find("3") != string::npos) saw3 = true;
        if (res.find("4") != string::npos) saw4 = true;
        if (res.find("5") != string::npos) saw5 = true;
        if (saw3 && saw4 && saw5) break;
    }

    EXPECT_TRUE(saw3);
    EXPECT_TRUE(saw4);
    EXPECT_TRUE(saw5);
}

TEST(GenerateTest, SameSeedSameResult)
{
    statetab tab;
    prefix start;
    start.push_back("доброе");
    start.push_back("утро");

    prefix k;
    k.push_back("доброе");
    k.push_back("утро");
    tab[k].push_back("солнце");

    setSeed(555);
    string r1 = generate(tab, start, 1);
    setSeed(555);
    string r2 = generate(tab, start, 1);

    EXPECT_EQ(r1, r2);
}

TEST(GenerateTest, WordSequence)
{
    statetab tab;
    prefix start;
    start.push_back("я");
    start.push_back("играю");

    prefix k1;
    k1.push_back("я");
    k1.push_back("играю");
    tab[k1].push_back("на");

    prefix k2;
    k2.push_back("играю");
    k2.push_back("на");
    tab[k2].push_back("площадке");

    setSeed(1);
    string res = generate(tab, start, 2);

    EXPECT_NE(res.find("я"), string::npos);
    EXPECT_NE(res.find("играю"), string::npos);
}

TEST(ConstantsTest, NPREFValue)
{
    EXPECT_EQ(NPREF, 2);
}

TEST(ConstantsTest, MAXGENValue)
{
    EXPECT_EQ(MAXGEN, 1000);
}

TEST(GenerateTest, StopWhenNoSuffix)
{
    statetab tab;
    prefix start;
    start.push_back("папа");
    start.push_back("мама");

    prefix k;
    k.push_back("папа");
    k.push_back("мама");
    tab[k].push_back("я");

    setSeed(1);
    string res = generate(tab, start, 100);

    int words = 1;
    for (char c : res) if (c == ' ') words++;

    EXPECT_EQ(words, 3);
}

TEST(BuildTableTest, FromFile)
{
    ofstream file("temp.txt");
    file << "1 2 3 4";
    file.close();

    statetab tab;
    prefix start;
    buildTable("temp.txt", tab, start);

    EXPECT_FALSE(tab.empty());
    EXPECT_EQ(start[0], "1");
    EXPECT_EQ(start[1], "2");

    prefix k;
    k.push_back("1");
    k.push_back("2");
    EXPECT_EQ(tab[k][0], "3");

    remove("temp.txt");
}
