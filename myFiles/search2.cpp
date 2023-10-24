// Do NOT add any other includes
#include "search.h"

#define XSIZE   20
#define ASIZE   256

void OUTPUT(Info* stringInfo, int k, Node* &curr, int&n_matches, unsigned long&currCharCount) {
    curr->right = new Node(stringInfo->a, stringInfo->b, stringInfo->c, stringInfo->d, (k+currCharCount));
    curr->right->left = curr;
    curr = curr->right;
    n_matches++;
}

int preColussi(string &pattern, int m, int h[], int next[], int shift[]) {
    int i, k, nd, q, r, s;
    int hmax[XSIZE], kmin[XSIZE], nhd0[XSIZE], rmin[XSIZE];

   /* Computation of hmax */
    i = k = 1;
    do {
        while (pattern[i] == pattern[i - k]) {
            i++;
        }
        hmax[k] = i;
        q = k + 1;
        while (hmax[q - k] + k < i) {
            hmax[q] = hmax[q - k] + k;
            q++;
        }
        k = q;
        if (k == i + 1) {
            i = k;
        }
    } while (k <= m);

   /* Computation of kmin */
    // memset(kmin, 0, m*sizeof(int));
    for (int i = 0; i < m; i++) {
        kmin[i] = 0;
    }
    for (i = m; i >= 1; --i) {
        if (hmax[i] < m) {
            kmin[hmax[i]] = i;
        }
    }
   /* Computation of rmin */
    for (i = m - 1; i >= 0; --i) {
        if (hmax[i + 1] == m) {
            r = i + 1;
        }
        if (kmin[i] == 0) {
            rmin[i] = r;
        }
        else {
            rmin[i] = 0;
        }
    }

   /* Computation of h */
    s = -1;
    r = m;
    for (i = 0; i < m; ++i) {
        if (kmin[i] == 0){
            h[--r] = i;
        }
        else {
            h[++s] = i;
        }
    }
    nd = s;

   /* Computation of shift */
    for (i = 0; i <= nd; ++i) {
        shift[i] = kmin[h[i]];
    }
    for (i = nd + 1; i < m; ++i) {
        shift[i] = rmin[h[i]];
    }
    shift[m] = rmin[0];

   /* Computation of nhd0 */
    s = 0;
    for (i = 0; i < m; ++i) {
        nhd0[i] = s;
        if (kmin[i] > 0) {
            ++s;
        } 
    }


   /* Computation of next */
    for (i = 0; i <= nd; ++i) {
        next[i] = nhd0[h[i] - kmin[h[i]]];
    }
    for (i = nd + 1; i < m; ++i) {
        next[i] = nhd0[m - rmin[h[i]]];
    }
    next[m] = nhd0[m - rmin[h[m - 1]]];

    return(nd);
}

void GG(Info* strInfo, string &pattern, Node* &curr, int&n_matches, unsigned long&currCharCount) {

    char t0 = (strInfo->s)[0]; char p0 = pattern[0];
    char*x = &p0; char*y = &t0;
    int m = pattern.length(); int n = (strInfo->s).length();

    int i, j, k, ell, last, nd;
    int h[XSIZE], next[XSIZE], shift[XSIZE];
    char heavy;

    for (ell = 0; pattern[ell] == pattern[ell + 1]; ell++);
    if (ell == m - 1) {
        /* Searching for a power of a single character */
        for (j = ell = 0; j < n; ++j)
            if (pattern[0] == (strInfo->s)[j]) {
                ++ell;
                if (ell >= m)
                OUTPUT(strInfo, (j - m+1), curr, n_matches, currCharCount);
            }
            else {
                ell = 0;
            }
    } else {
        /* Preprocessing */
        nd = preColussi(pattern, m, h, next, shift);

        /* Searching */
        i = j = heavy = 0;
        last = -1;
        while (j <= n - m) {
            if (heavy && i == 0) {
                k = last - j + 1;
                while (pattern[0] == (strInfo->s)[j + k]) {
                    k++;
                }
                if (k <= ell ||pattern[ell + 1] != (strInfo->s)[j + k]) {
                    i = 0;
                    j += (k + 1);
                    last = j - 1;
                }
                else {
                    i = 1;
                    last = j + k;
                    j = last - (ell + 1);
                }
                heavy = 0;
            }
            else {
                while (i < m && last < j + h[i] && pattern[h[i]] == (strInfo->s)[j + h[i]]) {
                    ++i;
                }
                if (i >= m || last >= j + h[i]) {
                    OUTPUT(strInfo, j, curr, n_matches, currCharCount);
                    i = m;
                }
                if (i > nd) {
                    last = j + m - 1;
                }
                j += shift[i];
                i = next[i];
            }
            heavy = (j > last ? 0 : 1);
        }
    }
    // cout << "gg DONE inside\n";
}

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
    Node* head = new Node(); //empty sentinel, will be deleted if n_matches!= 0 and next will be returned
    Node* curr = head;
    unsigned long currCharCount = 0;
    

    for (int i = 0; i < allStringsInfo.size(); i++) {
        GG(allStringsInfo[i], pattern, curr, n_matches, currCharCount);

        // cout << "gg DONE\n";
        currCharCount += (allStringsInfo[i]->s).length();
        // cout << currCharCount << "\n";
        // cout << curr->left->offset << "\n";
    }

    if (n_matches > 0) {
        Node* temp = head;
        head = head->right;
        head->left = NULL;
        delete temp;
        return head;
    } else {
        delete head; head = NULL;
        return nullptr;
    }    
}

// int main () {

//     SearchEngine SE;

//     SE.insert_sentence(1, 1, 1, 1, "Hello! This is a test sentence.");
//     SE.insert_sentence(1, 1, 2, 1, "Hello! This is another test sentence.");

//     for (Info* x: SE.allStringsInfo) {
//         cout << x->a << ", " << x->b << ", " << x->c << ", " << x->d << ", " << x->s << "\n";
//     }

//     int x = 0;
    
//     Node* result = SE.search("test", x);

//     while (result!=NULL) {
//         cout << x << " is the val of x\n"; 
//         result->display();
//         result = result->right;
//     }


//     return 0;
// }