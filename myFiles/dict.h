// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
class DictNode
{
public:
    string key;
    int wordCount = 0;
    DictNode *next = NULL;
};
class LList
{
public:
    DictNode *root;
    LList();
    ~LList();
    void insert(string key);
    DictNode *search(string &word);
};
class Chaining
{
public:
    vector<LList *> DictVec;
    Chaining();
    void insert(string word);
    unsigned long hash(const char *key, uint32_t len, uint32_t seed);
    DictNode *search(string &word);

    ~Chaining();

private:
    unsigned long siz;
    unsigned long num;
};
class Dict
{
private:
    // You can add attributes/helper functions here
    Chaining *DictList;

public:
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};