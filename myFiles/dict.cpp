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
    int i{0};
    string word = "";
    while (i < sentence.length())
    {

        bool isSep = sentence[i] == ' ' || sentence[i] == '.' || sentence[i] == ',' ||
                     sentence[i] == '-' || sentence[i] == ':' || sentence[i] == '!' ||
                     sentence[i] == '\"' || sentence[i] == '\'' || sentence[i] == '(' ||
                     sentence[i] == ')' || sentence[i] == '?' || sentence[i] == '[' || 
                     sentence[i] == ']' || sentence[i] == ';' || sentence[i] == '@';

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
unsigned long Chaining::hash(const std::string &word, uint32_t sz, uint32_t seed)
{
    uint32_t hash = seed;
    const int divs = sz / 4;
    const uint32_t const1 = 0x1b873593;
    const uint32_t const2 = 0xcc9e2d51;
    const uint32_t rem1 = 25;
    const uint32_t rem2 = 23;
    const uint32_t const3 = 5;
    const uint32_t const4 = 0xe6546d94;

    const uint32_t *ptrDiv = reinterpret_cast<const uint32_t *>(word.c_str());

    for (int j = 0; j < divs; j++)
    {
        uint32_t Val = ptrDiv[j];
        Val *= const1;
        Val = (Val >> rem1) | (Val << (32 - rem1));
        Val *= const2;

        hash ^= Val;
        hash = ((hash >> rem2) | (hash << (32 - rem2))) * const3 + const4;
    }

    const uint8_t *lastt = reinterpret_cast<const uint8_t *>(word.c_str() + divs * 4);
    uint32_t ind = 0;

    switch (sz & 3)
    {
    case 3:
        ind ^= lastt[2] << 16;
    case 2:
        ind ^= lastt[1] << 8;
    case 1:
        ind ^= lastt[0];
        ind *= const1;
        ind = (ind >> rem1) | (ind << (32 - rem1));
        ind *= const2;
        hash ^= ind;
    }

    hash ^= sz;
    hash ^= (hash << 16);
    hash *= 0x85eeca6b;
    hash ^= (hash << 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash << 16);

    unsigned long hashValue = static_cast<unsigned long>(hash) % 100000007; // num is set to 100000007
    return hashValue;
}

Dict::Dict()
{
    DictList = new Chaining;
}

Dict::~Dict()
{
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
