// Do NOT add any other includes
#include "search.h"

void createNode(Info* stringInfo, int k, Node* &curr, Node* &head, int&n_matches) {

    if (curr == NULL) {
        curr = new Node(stringInfo->a, stringInfo->b, stringInfo->c, stringInfo->d, (k));
        curr->left = curr->right = NULL;
        head = curr; //update only if first node is inserted
    } else {
        curr->right = new Node(stringInfo->a, stringInfo->b, stringInfo->c, stringInfo->d, (k));
        curr->right->right = NULL;
        curr->right->left = curr;
        curr = curr->right;
    }    
    n_matches++;
}

int setup(string &pattern, int m, vector<int> &hVec, vector<int> &nVec, vector<int> &patShiftVec) {
    int i = 0; int j = 0; 
    int noHoles = 0; int a, b, c;
    vector<int> arrH(m+1, 0);
    vector<int> arrK(m+1, 0);
    vector<int> smallerNoHoles(m+1, 0);
    vector<int> arrR(m+1, 0);

    i = 1; j = 1;
    while (j <= m) {
        while (pattern[i] == pattern[i - j]) {i++;}
        arrH[j] = i;
        a = j + 1;
        while (arrH[a - j] + j < i) {
            arrH[a] = arrH[a - j] + j;
            a++;
        }
        j = a;
        if (j == i + 1) {
            i = j;
        }
    }

    for (i = m; i >= 1; i--) {
        if (arrH[i] < m) {
            arrK[arrH[i]] = i;
        }
    }

    for (i = m - 1; i >= 0; i--) {
        if (arrH[i + 1] == m) {
            b = i + 1;
        } 
        if (arrK[i] == 0) {
            arrR[i] = b;
        } else {
            arrR[i] = 0;
        }
    }

    c = -1; b = m;
    for (i = 0; i < m; i++) {
        if (arrK[i] == 0){
            hVec[--b] = i;
        }
        else {
            hVec[++c] = i;
        }
    }
    noHoles = c + 1;

    for (i = 0; i < noHoles; i++) {
        patShiftVec[i] = arrK[hVec[i]];
    }
    for (i = noHoles; i < m; i++) {
        patShiftVec[i] = arrR[hVec[i]];
    }

    patShiftVec[m] = arrR[0];

    c = 0;
    i = 0;
    while (i < m) {
        smallerNoHoles[i] = c;
        if (arrK[i] > 0) {c++;} 
        i++;
    }

    for (i = 0; i < noHoles; ++i) {
        nVec[i] = smallerNoHoles[hVec[i] - arrK[hVec[i]]];
    }

    for (i = noHoles; i < m; ++i) {
        nVec[i] = smallerNoHoles[m - arrR[hVec[i]]];
    }
    nVec[m] = smallerNoHoles[m - arrR[hVec[m - 1]]];

    return noHoles;
}

void GalilGiancarlo(Info* strInfo, string &pattern, Node* &curr, Node* &head, int&n_matches) {

    int m = pattern.length(); int n = (strInfo->s).length();

    int i = 0; int j = 0; int k = 0; int lBorderIdx = 0; int noHoles = 0;
    int counter = 0;
    vector<int> hVec(m+1);
    vector<int> nVec(m+1);
    vector<int> patShiftVec(m+1);
    bool x;

    while (pattern[counter] == pattern[counter + 1]) {
        counter++;
    } //finds repeated power of single char in pattern

    if (counter == m - 1) {
        //to find whether single char repeated throughout string: GG not applicable
        counter = 0;
        for (j = 0; j < n; ++j)
            if (pattern[0] == (strInfo->s)[j]) {
                counter++;
                if (counter >= m)
                createNode(strInfo, (j - m+1), curr, head, n_matches); //match found
            } else {
                counter = 0;
            }
    } else {
        //setup : O(m)
        noHoles = setup(pattern, m, hVec, nVec, patShiftVec);

        //Search : O(n)
        i = 0; j = 0; x = 0;
        lBorderIdx = -1;
        while (j <= n - m) {

            if (x!=0 && i == 0) {
                k = lBorderIdx - j + 1;
                while (pattern[0] == (strInfo->s)[j + k]) {k++;}
                if (k <= counter || pattern[counter + 1] != (strInfo->s)[j + k]) {
                    i = 0;
                    j += (k + 1); lBorderIdx = j - 1;
                }
                else {
                    i = 1;
                    lBorderIdx = j + k; j = lBorderIdx - counter + 1;
                }
                x = 0;
            }

            else {
                while (i < m && lBorderIdx < j + hVec[i] && pattern[hVec[i]] == (strInfo->s)[j + hVec[i]]) {
                    i++;
                }
                if (i >= m || lBorderIdx >= j + hVec[i]) {
                    createNode(strInfo, j, curr, head, n_matches); //match found
                    i = m;
                }
                if (i > noHoles - 1) {
                    lBorderIdx = j + m - 1;
                }
                j += patShiftVec[i];
                i = nVec[i];
            }
            if (j>lBorderIdx) {x = 0;} 
            else {x = 1;}
        }
    }
}

//=========================================================

SearchEngine::SearchEngine(){
    // Required stuff is pre-initialized
}

SearchEngine::~SearchEngine(){
    while (allStringsInfo.size() > 0) {
        Info* temp = allStringsInfo.back();
        allStringsInfo.pop_back();
        delete temp; temp = NULL;
    }
}

void convertToLowerCase(string &sentence) {
    for (char &x : sentence) {
        if (int(x) >= 65 && int(x) <=90) {
            x = char(int(x) + 32);
        }
    }
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){ //checked, works
    convertToLowerCase(sentence);
    allStringsInfo.push_back(new Info(book_code, page, paragraph, sentence_no, sentence)); 
    //processing, ie, storing. This function will be called for all sentences before search is used
    return;
}

Node* SearchEngine::search(string pattern, int& n_matches){
    Node* head = NULL;
    Node* curr = NULL;
    convertToLowerCase(pattern);
    

    for (int i = 0; i < allStringsInfo.size(); i++) {
        GalilGiancarlo(allStringsInfo[i], pattern, curr, head, n_matches);
    }

    return head; //NULL if no matches found
}