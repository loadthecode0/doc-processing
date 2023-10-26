// Do NOT add any other includes
#include "dict.h"

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

Chaining::Chaining()
{
    siz = 0;

    num = 100663319;
    int i{0};

    DictVec.resize(num);
}

Chaining::~Chaining()
{

    for (int i{0}; i < DictVec.size(); i++)
    {
        for (int j{0}; j < DictVec[i].size(); j++)
        {
            delete DictVec[i][j];
        }
    }
}
void Chaining::insert(string word)
{

    unsigned long hashCode = hash(word, word.size(), 1);

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
}
DictNode *Chaining::search(string &word)
{

    unsigned long hashCode = hash(word, word.size(), 1);
    return findDict(word, DictVec[hashCode]);
}
unsigned long Chaining::hash(const string &word, uint32_t len, uint32_t seed)
{
    const uint32_t const1 = 0xcc9e2d51;
    const uint32_t const2 = 0x1b873593;
    const uint32_t rem1 = 15;
    const uint32_t rem2 = 13;
    const uint32_t const3 = 5;
    const uint32_t const4 = 0xe6546b64;

    uint32_t hash = seed;
    const int nblocks = len / 4;
    const uint32_t *blocks = reinterpret_cast<const uint32_t *>(word.c_str());

    for (int i = 0; i < nblocks; i++)
    {
        uint32_t Val = blocks[i];
        Val *= const1;
        Val = (Val << rem1) | (Val >> (32 - rem1));
        Val *= const2;

        hash ^= Val;
        hash = ((hash << rem2) | (hash >> (32 - rem2))) * const3 + const4;
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
        k1 *= const1;
        k1 = (k1 << rem1) | (k1 >> (32 - rem1));
        k1 *= const2;
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
    DictList = new Chaining;
}

Dict::~Dict()
{
    dump_dictionary("output.txt");
    delete DictList;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{

    vector<string> words = splitSentenceIntoWords(sentence);
    for (auto iniword : words)
    {
        string word = "";
        for (auto x : iniword)
        {
            word += tolower(x);
        }
        DictList->insert(word);
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

    DictNode *node = DictList->search(newword);
    if (node != NULL)
    {
        return node->wordCount;
    }

    return -1;
}

void Dict::dump_dictionary(string filename)
{

    ofstream file;

    file.open(filename);
    for (int i{0}; i < DictList->DictVec.size(); i++)
    {

        for (int j{0}; j < DictList->DictVec[i].size(); j++)
        {
            file << DictList->DictVec[i][j]->key << ", " << this->get_word_count(DictList->DictVec[i][j]->key) << endl;
        }
    }
    file.close();
    return;
}
