#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdint.h>
#include <unordered_map>

// std::unordered_set<uint32_t> occurred;
// uint32_t collisionCount = 0;
// unsigned long long wordCount = 0;

// uint32_t customHash(const char *key, uint32_t len, uint32_t seed) {
//     const uint32_t c1 = 0xcc9e2d51;
//     const uint32_t c2 = 0x1b873593;
//     const uint32_t r1 = 15;
//     const uint32_t r2 = 13;
//     const uint32_t m = 5;
//     const uint32_t n = 0xe6546b64;

//     uint32_t hash = seed;
//     const int nblocks = len / 4;
//     const uint32_t *blocks = (const uint32_t *)key;
//     int i;

//     for (i = 0; i < nblocks; i++) {
//         uint32_t k = blocks[i];
//         k *= c1;
//         k = (k << r1) | (k >> (32 - r1));
//         k *= c2;

//         hash ^= k;
//         hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
//     }
//     const uint8_t *tail = (const uint8_t *)(key + nblocks * 4);
//     uint32_t k1 = 0;

//     switch (len & 3) {
//         case 3:
//             k1 ^= tail[2] << 16;
//         case 2:
//             k1 ^= tail[1] << 8;
//         case 1:
//             k1 ^= tail[0];
//             k1 *= c1;
//             k1 = (k1 << r1) | (k1 >> (32 - r1));
//             k1 *= c2;
//             hash ^= k1;
//     }

//     hash ^= len;
//     hash ^= (hash >> 16);
//     hash *= 0x85ebca6b;
//     hash ^= (hash >> 13);
//     hash *= 0xc2b2ae35;
//     hash ^= (hash >> 16);

//     return hash;

// }

std::vector<std::string> splitSentenceIntoWords(const std::string &sentence)
{
    std::vector<std::string> words;
    std::istringstream iss(sentence);
    std::string word;
    while (iss >> word)
    {
        words.push_back(word);
    }
    return words;
}

// void process(const std::string &word) {
//     char c = word[0];
//     char*c1 = &c;
//     unsigned long hashCode = hashF(c1, word.size(), 1);
//     wordCount++;
//     if (occurred.count(hashCode) > 0) {
//         collisionCount++;
//         std::cout << word << " " << hashCode << "\n";
//     } else {
//         occurred.insert(hashCode);
//     }

// }

// int main() {
//     std::string filename = "mahatma-gandhi-collected-works-volume-1.txt";
//     std::ifstream file(filename);

//     if (!file.is_open()) {
//         std::cerr << "Error opening file: " << filename << std::endl;
//         return 1;
//     }

//     std::string line;
//     while (std::getline(file, line)) {
//         std::vector<std::string> words = splitSentenceIntoWords(line);
//         for (const std::string &word : words) {
//             process(word);
//         }
//     }

//     file.close();

//     if (wordCount > 0) {
//         double collisionPercentage = (static_cast<double>(collisionCount) / wordCount) * 100;
//         std::cout << "Collision percentage: " << collisionPercentage << "%" << std::endl;
//     } else {
//         std::cout << "No words to process." << std::endl;
//     }

//     return 0;
// }

// #include <iostream>
// #include <unordered_set>
#include <random>
#include <functional>
#include <ctime>

// Define your hash function here

unsigned long customHash(const char *key, uint32_t len, uint32_t seed)
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

    return (unsigned long)hash;
}

// Function to test collision rate
double testCollisionRate()
{
    // std::unordered_set<unsigned long> hashSet;
    unsigned long long numInputs = 0;
    unsigned long long colCount = 0;
    std::unordered_map<unsigned long, std::vector<std::string>> mapp;
    std::unordered_map<unsigned long, bool> hashSet;
    // std::default_random_engine generator(static_cast<unsigned>(time(nullptr)));
    // std::uniform_int_distribution<char> charDistribution('a', 'z');
    // auto randomChar = std::bind(charDistribution, generator);
    //=======================================
    std::string filename = "mahatma-gandhi-collected-works-volume-1.txt";
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    std::string line;
    std::unordered_set<std::string> uniqueWords;
    while (std::getline(file, line))
    {
        std::vector<std::string> words = splitSentenceIntoWords(line);
        for (std::string word : words)
        {
            if (uniqueWords.find(word) == uniqueWords.end())
            {
                uniqueWords.insert(word);
            }
        }
    }
    file.close();
    // while (std::getline(file, line))
    // {
    //     std::vector<std::string> words = splitSentenceIntoWords(line);
    //     for (std::string word : words)
    //     {
    //         if (uniqueWords.find(word) == uniqueWords.end())
    //         {
    //             uniqueWords.insert(word);
    //         }
    //     }
    for (const std::string &word : uniqueWords)
    {
        numInputs++;
        char ch = char(word[0]);
        char *c = &ch;
        unsigned long hash = customHash(c, word.size(), 1);

        if (hashSet.find(hash) != hashSet.end())
        {
            // Collision detected
            colCount++;

            // std::cout << "Collision: " << word << " - Hash: " << hash << std::endl;
            mapp[hash].push_back(word);
        }
        else
        {
            hashSet.insert({hash, 1});
            mapp[hash].push_back(word);
        }
        // for (auto it = mapp.begin(); it != mapp.end(); it++)
        // {
        //     std::cout << "hash value is" << it->first << ": ";
        //     for (auto x : it->second)
        //     {
        //         std::cout << x << " ";
        //     }
        //     std::cout << std::endl;
        // }
    }

    double collisionRate = static_cast<double>(colCount) / numInputs;
    return collisionRate;
}

//============================
// for (unsigned int i = 0; i < numInputs; ++i) {
//     std::string randomInput;
//     for (unsigned int j = 0; j < inputLength; ++j) {
//         randomInput.push_back(randomChar());
//     }

int main()
{
    // unsigned int numInputs = 10000;  // Number of random inputs to test
    // unsigned int inputLength = 10;   // Length of each random input

    double collisionRate = testCollisionRate();
    std::cout << "Collision rate: " << collisionRate * 100 << "%" << std::endl;

    return 0;
}