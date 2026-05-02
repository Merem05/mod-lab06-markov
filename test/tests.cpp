#include <gtest/gtest.h>
#include "textgen.h"

TEST(PrefixTest, ThreeWords) {
    prefix p;
    p.push_back("city");
    p.push_back("village");
    p.push_back("country");

    EXPECT_EQ(p.size(), 3);
    EXPECT_EQ(p[0], "city");
    EXPECT_EQ(p[1], "village");
    EXPECT_EQ(p[2], "country");
}

TEST(PrefixTest, TwoWords) {
    prefix p;
    p.push_back("white");
    p.push_back("red");

    EXPECT_EQ(p.size(), 2);
    EXPECT_EQ(p[0], "white");
    EXPECT_EQ(p[1], "red");
}

TEST(StateTabTest, PrefixSuffixRecord) {
    statetab tab;
    prefix key;
    key.push_back("mother");
    key.push_back("cooked");

    tab[key].push_back("soup");
    tab[key].push_back("dinner");

    EXPECT_EQ(tab[key].size(), 2);
    EXPECT_EQ(tab[key][0], "soup");
    EXPECT_EQ(tab[key][1], "dinner");
}

TEST(StateTabTest, SingleSuffix) {
    statetab tab;
    prefix key;
    key.push_back("blue");
    key.push_back("green");
    tab[key].push_back("pink");

    EXPECT_EQ(tab[key].size(), 1);
    EXPECT_EQ(tab[key][0], "pink");
}

TEST(StateTabTest, MultipleSuffixes) {
    statetab tab;
    prefix key;
    key.push_back("bread");
    key.push_back("butter");

    tab[key].push_back("cheese");
    tab[key].push_back("milk");
    tab[key].push_back("sour cream");

    EXPECT_EQ(tab[key].size(), 3);
}

TEST(GenerateTest, ManualTable) {
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

TEST(GenerateTest, ZeroLength) {
    statetab tab;
    prefix start;

    start.push_back("a");
    start.push_back("b");

    prefix k;
    k.push_back("a");
    k.push_back("b");
    tab[k].push_back("c");

    setSeed(1);
    string res = generate(tab, start, 0);

    int words = 1;
    for (char c : res) {
        if (c == ' ') ++words;
    }

    EXPECT_EQ(words, 2);
}

TEST(GenerateTest, EmptyTable) {
    statetab empty;
    prefix start;

    string res = generate(empty, start, 10);

    EXPECT_TRUE(res.empty());
}

TEST(GenerateTest, RandomSuffixSelection) {
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

    for (int i = 1; i <= 20; ++i) {
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

TEST(GenerateTest, SameSeedSameResult) {
    statetab tab;
    prefix start;
    start.push_back("good");
    start.push_back("morning");

    prefix k;
    k.push_back("good");
    k.push_back("morning");
    tab[k].push_back("sun");

    setSeed(555);
    string r1 = generate(tab, start, 1);
    setSeed(555);
    string r2 = generate(tab, start, 1);

    EXPECT_EQ(r1, r2);
}

TEST(GenerateTest, WordSequence) {
    statetab tab;
    prefix start;
    start.push_back("I");
    start.push_back("play");

    prefix k1;
    k1.push_back("I");
    k1.push_back("play");
    tab[k1].push_back("on");

    prefix k2;
    k2.push_back("play");
    k2.push_back("on");
    tab[k2].push_back("playground");

    setSeed(1);
    string res = generate(tab, start, 2);

    EXPECT_NE(res.find("I"), string::npos);
    EXPECT_NE(res.find("play"), string::npos);
}

TEST(ConstantsTest, NPREFValue) {
    EXPECT_EQ(NPREF, 2);
}

TEST(ConstantsTest, MAXGENValue) {
    EXPECT_EQ(MAXGEN, 1000);
}

TEST(GenerateTest, StopWhenNoSuffix) {
    statetab tab;
    prefix start;
    start.push_back("dad");
    start.push_back("mom");

    prefix k;
    k.push_back("dad");
    k.push_back("mom");
    tab[k].push_back("me");

    setSeed(1);
    string res = generate(tab, start, 100);

    int words = 1;
    for (char c : res) {
        if (c == ' ') ++words;
    }

    EXPECT_EQ(words, 3);
}

TEST(BuildTableTest, FromFile) {
    std::ofstream file("temp.txt");
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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
