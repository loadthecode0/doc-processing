// Do NOT add any other includes
#include "search.h"

#define XSIZE   20
#define ASIZE   256




void preMp(char *x, int m, int mpNext[]) {
    int i, j;
    
    i = 0;
    j = mpNext[0] = -1;
    while (i < m) {
        while (j > -1 && x[i] != x[j])
        j = mpNext[j];
        mpNext[++i] = ++j;
    }
}


void OUTPUT(Info* stringInfo, int k, Node* &curr, int&n_matches, unsigned long&currCharCount) {
    curr->right = new Node(stringInfo->a, stringInfo->b, stringInfo->c, stringInfo->d, (k+currCharCount));
    curr->right->left = curr;
    curr = curr->right;
    n_matches++;
}

//int book_code, int page, int paragraph, int sentence_no, string &text, string &pattern, Node* &curr, int&n_matches, unsigned long&currCharCount
void MTKGalilGiancarloSearch(Info* strInfo, string &pattern, Node* &curr, int&n_matches, unsigned long&currCharCount) {
    
    char t0 = (strInfo->s)[0]; char p0 = pattern[0];
    char*x = &p0; char*y = &t0;
    int m = pattern.length(); int n = (strInfo->s).length();

    cout << *x << " " << m << " " << *y << " " << n << "\n";

    int i =0, j=0;
/* Searching */
    for (j = 0; j <= n - m; j++) {
        
        while (i<m && pattern[i] == (strInfo->s)[i + j]) {
            cout << pattern[i] << " " <<(strInfo->s)[i+j] << "\n";
            i++;
        }
        if (i >= m) {
            cout << "j is " << j << "\n";
            OUTPUT(strInfo, j, curr, n_matches, currCharCount); 
            break;
        }   
    }
}


//=========================================================

SearchEngine::SearchEngine(){
    // Implement your function here  
    

}

SearchEngine::~SearchEngine(){
    // Implement your function here  
    while (allStringsInfo.size() > 0) {
        Info* temp = allStringsInfo.back();
        allStringsInfo.pop_back();
        delete temp; temp = NULL;
    }
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){ //checked, works
    // Implement your function here  
    allStringsInfo.push_back(new Info(book_code, page, paragraph, sentence_no, sentence)); 
    //processing, ie, storing. This function will be called for all sentences before search is used
    return;
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here  
    Node* head = new Node();
    Node* curr = head;
    unsigned long currCharCount = 0;
    

    for (int i = 0; i < allStringsInfo.size(); i++) {
        MTKGalilGiancarloSearch(allStringsInfo[i], pattern, curr, n_matches, currCharCount);

        cout << "gg DONE\n";
        currCharCount += (allStringsInfo[i]->s).length();
        cout << currCharCount << "\n";
        cout << curr->left->offset << "\n";
    }

    if (n_matches > 0) {
        Node* temp = head;
        head = head->right;
        head->left = NULL;
        delete temp;
        return head;
    } else {
        return nullptr;
    }    
}

int main () {

    SearchEngine SE;

    SE.insert_sentence(1, 1, 1, 1, "Hello! This is a test sentence.");
    SE.insert_sentence(1, 1, 2, 1, "Hello! This is another test sentence.");

    for (Info* x: SE.allStringsInfo) {
        cout << x->a << ", " << x->b << ", " << x->c << ", " << x->d << ", " << x->s << "\n";
    }

    int x = 0;
    
    Node* result = SE.search("test", x);

    while (result!=NULL) {
        cout << x << " is the val of x\n"; 
        result->display();
        result = result->right;
    }


    return 0;
}