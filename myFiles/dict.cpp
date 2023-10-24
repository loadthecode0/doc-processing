// Do NOT add any other includes
#include "dict.h"
#include <chrono>
using namespace std::chrono;
#include <sstream> //need to remove
DictNode *findDict(string word, vector<DictNode *> &vec)
{
    for (int i{0}; i < vec.size(); i++)
    {
        if (vec[i]->key == word)
        {
            return vec[i];
        }
    }
    return NULL;
}

vector<string> splitSentenceIntoWords(const string &sentence)
{
    vector<string> words;

    string separators = " .,-:!\"\'()?˙[];@";
    int i{0};
    string word = "";
    while (i < sentence.length())
    {

        bool isSep = sentence[i] == ' ' || sentence[i] == '.' || sentence[i] == ',' ||
                     sentence[i] == '-' || sentence[i] == ':' || sentence[i] == '!' ||
                     sentence[i] == '\"' || sentence[i] == '\'' || sentence[i] == '(' ||
                     sentence[i] == ')' || sentence[i] == '?' ||
                     sentence[i] == '[' || sentence[i] == ']' || sentence[i] == ';' ||
                     sentence[i] == '@';

        if (!isSep && i != sentence.length() - 1)
        {
            word += sentence[i];
        }
        else if (i == sentence.length() - 1)
        {
            if (!isSep)
            {
                word += sentence[i];
            }

            if (word != "")
            {
                words.push_back(word);
                word = "";
            }
        }
        else
        {
            if (word != "")
            {
                words.push_back(word);
                word = "";
            }
        }
        i++;
    }

    return words;
}
DictNode::DictNode()
{
    key = "";
    wordCount = 0;
    next = NULL;
}
DictNode::~DictNode()
{
    key = "";
    wordCount = 0;
    next = NULL;
    // cout << "node des" << endl;
}
// LList::LList()
// {
//     root = NULL;
// }
// LList::~LList()
// {
//     DictNode *temp = root;
//     while (temp != NULL)
//     {
//         DictNode *temp2 = temp->next;
//         temp->next = NULL;
//         delete temp;
//         temp = temp2;
//     }
//     cout << "LList destructor called" << endl;
// }
// void LList::insert(string key)
// {
//     if (root == NULL)
//     {
//         DictNode *newNode = new DictNode();
//         newNode->key = key;
//         newNode->wordCount++;
//         root = newNode;

//         root->next = NULL;
//     }
//     else
//     {
//         DictNode *temp = root;
//         while (temp->key != key && temp->next != NULL)
//         {
//             temp = temp->next;
//         }
//         if (temp->key == key)
//         {
//             temp->wordCount++;
//         }
//         else
//         {
//             DictNode *newNode = new DictNode();
//             newNode->key = key;
//             newNode->wordCount++;
//             temp->next = newNode;
//         }
//     }
// }
// DictNode *LList::search(string &word)
// {
//     DictNode *temp = this->root;

//     while (temp != NULL)
//     {
//         if (temp->key == word)
//         {
//             return temp;
//         }
//         temp = temp->next;
//     }

//     return temp;
// }

Chaining::Chaining()
{
    siz = 0;
    // num = 4294967296;
    num = 100000007;
    int i{0};

    // vector<DictNode *> vec; // Set your desired initial value

    DictVec.resize(num);
    // while (i < num)
    // {
    //     // LList *temp = new LList();
    //     vector<DictNode *> vec;
    //     DictVec.push_back(vec);
    //     i++;
    // }
}

Chaining::~Chaining()
{
    // for (int i{0}; i < DictVec.size(); i++)
    // {
    //     delete DictVec[i];
    // }
    for (int i{0}; i < DictVec.size(); i++)
    {
        for (int j{0}; j < DictVec[i].size(); j++)
        {
            delete DictVec[i][j];
        }
    }
    // cout << "chaining destructor called" << endl;
}
void Chaining::insert(string word)
{

    unsigned long hashCode = hash(word, word.size(), 1);
    // cout << "insert hash code " << word << " " << hashCode << endl;
    if (!DictVec[hashCode].empty())
    {
        DictNode *temp = findDict(word, DictVec[hashCode]);
        if (temp != NULL)
        {
            temp->wordCount++;
        }
        else
        {
            DictNode *newNode = new DictNode();
            newNode->key = word;
            newNode->wordCount = 1;
            DictVec[hashCode].push_back(newNode);
        }
    }
    else
    {
        DictNode *newNode = new DictNode();
        newNode->key = word;
        newNode->wordCount = 1;

        DictVec[hashCode].push_back(newNode);
    }
    // DictVec[hashCode]->insert(word);
}
DictNode *Chaining::search(string &word)
{

    unsigned long hashCode = hash(word, word.size(), 1);
    return findDict(word, DictVec[hashCode]);
}
unsigned long Chaining::hash(const string &word, uint32_t len, uint32_t seed)
{
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const uint32_t r1 = 15;
    const uint32_t r2 = 13;
    const uint32_t m = 5;
    const uint32_t n = 0xe6546b64;

    uint32_t hash = seed;
    const int nblocks = len / 4;
    const uint32_t *blocks = reinterpret_cast<const uint32_t *>(word.c_str());

    for (int i = 0; i < nblocks; i++)
    {
        uint32_t k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = reinterpret_cast<const uint8_t *>(word.c_str() + nblocks * 4);
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

    unsigned long hashValue = static_cast<unsigned long>(hash) % 100000007; // num is set to 100000007
    return hashValue;
}

Dict::Dict()
{
    // Implement your function here
}

Dict::~Dict()
{

    // Implement your function here
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here

    vector<string> words = splitSentenceIntoWords(sentence);
    for (auto iniword : words)
    {
        string word = "";
        for (auto x : iniword)
        {
            word += tolower(x);
        }
        DictList.insert(word);
    }
    return;
}

int Dict::get_word_count(string word)
{
    string newword = "";
    for (auto x : word)
    {
        newword += tolower(x);
    }
    // DictNode *node = DictList->DictVec->search(newword);

    // unsigned long hashCode = DictList->hash(newword, newword.size(), 1);
    //  cout << "hash code in get word " << newword << " " << hashCode << endl;
    //  DictNode *temp = DictList->DictVec[hashCode]->root;
    //  unsigned long ans{0};
    //  while (temp != NULL)
    //  {
    //      // cout << temp->key << " " << temp->wordCount << endl;
    //      if (temp->key == word)
    //      {
    //          // cout << "entered" << temp->key << " " << word << temp->wordCount << endl;
    //          return temp->wordCount;
    //      }
    //      temp = temp->next;
    //  }
    DictNode *node = DictList.search(newword);
    if (node != NULL)
    {
        return node->wordCount;
    }

    return -1;
}

void Dict::dump_dictionary(string filename)
{
    // Implement your function here
    ofstream file;

    file.open(filename);
    for (int i{0}; i < DictList.DictVec.size(); i++)
    {
        // DictNode *temp = DictList->DictVec[i]->root;
        // while (temp != NULL)
        // {
        //     file << temp->key << ", " << this->get_word_count(temp->key) << endl;
        //     temp = temp->next;
        // }

        for (int j{0}; j < DictList.DictVec[i].size(); j++)
        {
            file << DictList.DictVec[i][j]->key << ", " << this->get_word_count(DictList.DictVec[i][j]->key) << endl;
        }
    }
    file.close();
    return;
}
#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

// int main()
// {

//     std::ifstream inputFile(FILENAME);

//     if (!inputFile.is_open())
//     {
//         std::cerr << "Error: Unable to open the input file." << std::endl;
//         return 1;
//     }

//     std::string tuple;
//     std::string sentence;

//     Dict *d = new Dict();

//     // cout << "1" << endl;

//     while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
//     {
//         // Get a line in the sentence
//         tuple += ')';

//         std::vector<int> metadata;
//         std::istringstream iss(tuple);

//         // Temporary variables for parsing
//         std::string token;

//         // Ignore the first character (the opening parenthesis)
//         iss.ignore(1);

//         // Parse and convert the elements to integers
//         while (std::getline(iss, token, ','))
//         {

//             // Trim leading and trailing white spaces
//             size_t start = token.find_first_not_of(" ");
//             size_t end = token.find_last_not_of(" ");
//             if (start != std::string::npos && end != std::string::npos)
//             {
//                 token = token.substr(start, end - start + 1);
//             }

//             // Check if the element is a number or a string
//             if (token[0] == '\'')
//             {
//                 // Remove the single quotes and convert to integer
//                 int num = std::stoi(token.substr(1, token.length() - 2));
//                 metadata.push_back(num);
//             }
//             else
//             {
//                 // Convert the element to integer
//                 int num = std::stoi(token);
//                 metadata.push_back(num);
//             }
//         }

//         // Now we have the string in sentence
//         // And the other info in metadata
//         // Add to the dictionary

//         // Insert in the dictionary

//         d->insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
//     }

//     inputFile.close();

//     // d->dump_dictionary("outputCount.txt");
//     // string sentence1 = "Characterisctic, this is Sneha here!";
//     // string sentence2 = "Characterisctic, this is Aastha here!";
//     // string sentence3 = "Characterisctic, This is A here!";
//     // string sentence4 = "Characterisctic, this is B here!";
//     // Dict d;
//     // d.insert_sentence(0, 0, 0, 0, sentence1);
//     // d.insert_sentence(0, 0, 0, 0, sentence2);
//     // d.insert_sentence(0, 0, 0, 0, sentence3);
//     // d.insert_sentence(0, 0, 0, 0, sentence4);
//     // d.get_word_count("Characterisctic");
//     // d.get_word_count("Characterisctic");
//     // d.get_word_count("Characterisctic");
//     // d.get_word_count("this");

//     delete d;

//     return 0;
// }
