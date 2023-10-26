// Do NOT add any other includes
#include "search.h"

#include<bits/stdc++.h>

void OUTPUT(Info* stringInfo, int k, Node* &curr, Node* &head, int&n_matches) {

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

int preColussi(string &pattern, int m, int h[], int next[], int shift[]) {
    int i, k, nd, q, r, s;
    int XSIZE = m;
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

void GG(Info* strInfo, string &pattern, Node* &curr, Node* &head, int&n_matches) {

    char t0 = (strInfo->s)[0]; char p0 = pattern[0];
    char*x = &p0; char*y = &t0;
    int m = pattern.length(); int n = (strInfo->s).length();

    //FIX THIS
    int XSIZE = m+1;

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
                OUTPUT(strInfo, (j - m+1), curr, head, n_matches);
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
                    OUTPUT(strInfo, j, curr, head, n_matches);
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
        GG(allStringsInfo[i], pattern, curr, head, n_matches);
    }

    return head; //NULL if no matches found
}


// #define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

// int main() {
//     std::ifstream inputFile(FILENAME);

//     if (!inputFile.is_open()) {
//         std::cerr << "Error: Unable to open the input file." << std::endl;
//         return 1;
//     }

//     std::string tuple;
//     std::string sentence;

//     SearchEngine d;

//     while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
//         // Get a line in the sentence
//         tuple += ')';

//         std::vector<int> metadata;    
//         std::istringstream iss(tuple);

//         // Temporary variables for parsing
//         std::string token;

//         // Ignore the first character (the opening parenthesis)
//         iss.ignore(1);

//         // Parse and convert the elements to integers
//         while (std::getline(iss, token, ',')) {
//             // Trim leading and trailing white spaces
//             size_t start = token.find_first_not_of(" ");
//             size_t end = token.find_last_not_of(" ");
//             if (start != std::string::npos && end != std::string::npos) {
//                 token = token.substr(start, end - start + 1);
//             }
            
//             // Check if the element is a number or a string
//             if (token[0] == '\'') {
//                 // Remove the single quotes and convert to integer
//                 int num = std::stoi(token.substr(1, token.length() - 2));
//                 metadata.push_back(num);
//             } else {
//                 // Convert the element to integer
//                 int num = std::stoi(token);
//                 metadata.push_back(num);
//             }
//         }

//         // Now we have the string in sentence
//         // And the other info in metadata
//         // Add to the dictionary

//         // Insert in the dictionary
//         d.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
//     }

//     inputFile.close();

//     // for (Info* &x: d.allStringsInfo) {
//     //     cout << x->a << ", " << x->b << ", " << x->c << ", " << x->d << ", " << x->s << "\n";
//     // }

//     int x = 0;
    
//     Node* result = d.search("well", x);

//     Node* dispRes = result;

//     while (dispRes!=NULL) {
//         cout << x << " is the val of x\n"; 
//         cout << dispRes->page << " " << dispRes->paragraph<<" " << dispRes->sentence_no << " " << dispRes->offset <<"\n";
//         dispRes = dispRes->right;
//     }

    

//     while(result != NULL) {
//         Node* temp = result;
//         result = result->right;
//         delete temp; temp = NULL;
//     }

//     if (result!=NULL) {
//         delete result; result = NULL;
//     }

//     return 0;
// }