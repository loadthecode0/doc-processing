#include <bits/stdc++.h>
using namespace std;

vector<string> splitSentenceIntoWords(const string &sentence)
{
    vector<string> words;

    string separators = " .,-:!\"\'()?˙[];@";
    int i{0};
    int j{0};
    string word = "";
    while (i < sentence.length())
    {
        // if (i == sentence.length() - 1)
        // {
        if (sentence[i] != separators[0] && sentence[i] != separators[1] && sentence[i] != separators[2] &&
            sentence[i] != separators[3] && sentence[i] != separators[4] && sentence[i] != separators[5] &&
            sentence[i] != separators[7] && sentence[i] != separators[9] && sentence[i] != separators[10] &&
            sentence[i] != separators[11] && sentence[i] != separators[12] && sentence[i] != separators[13] &&
            sentence[i] != separators[14] && sentence[i] != separators[15] && sentence[i] != separators[16] &&
            sentence[i] != separators[17] && i != sentence.length() - 1)
        {

            word += sentence[i];
        }
        else
        {
            words.push_back(word);
            word = ""; // reset
        }
        // }
        // else
        // {
        //     if (sentence[i] == separators[0] || sentence[i] == separators[1] || sentence[i] == separators[2] || sentence[i] == separators[3] || sentence[i] == separators[4] || sentence[i] == separators[5] || sentence[i] == separators[6] || sentence[i] == separators[7] || sentence[i] == separators[8] || sentence[i] == separators[9] || sentence[i] == separators[10] || sentence[i] == separators[11] || sentence[i] == separators[12] || sentence[i] == separators[13] || sentence[i] == separators[14] || sentence[i] == separators[15] || sentence[i] == separators[16])
        //     {
        //         string word = "";
        //         for (int k{j}; k < i; k++)
        //         {
        //             word += sentence[k];
        //         }
        //         if (word != "")
        //         {
        //             words.push_back(word);
        //         }
        //         j = i + 1;
        //     }
        //
        // }
        i++;
    }

    return words;
}

unsigned long hash(const char *key, uint32_t len, uint32_t seed)
{
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const uint32_t r1 = 15;
    const uint32_t r2 = 13;
    const uint32_t m = 5;
    const uint32_t n = 0xe6546b64;

    uint32_t hash = seed;
    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *)key;
    int i;

    for (i = 0; i < nblocks; i++)
    {
        uint32_t k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = (const uint8_t *)(key + nblocks * 4);
    uint32_t k1 = 0;

    switch (len & 3)
    {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (32 - r1));
        k1 *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return (unsigned long)hash % 10000007;
}

int main()
{

    vector<string> v = splitSentenceIntoWords("Hello, there. Nice to meet ya ! Be original");
    for (string s:v) {
        cout << s << " ";
    }
    return 0;
}