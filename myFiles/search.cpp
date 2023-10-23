// Do NOT add any other includes
#include "search.h"

//implementation of automata
class _cell {
    public:
    int element;
    _cell *next;
};

typedef _cell *List;

class _graph {
    public:
    int vertexNumber,
    edgeNumber,
    vertexCounter,
    initial,
    *terminal,
    *target,
    *suffixLink,
    *length,
    *position,
    *shift;
};
typedef _graph *Graph; //ie Graph is pointer to _graph; Graph is alias for _graph*
typedef int boolean;

#define UNDEFINED -1
/* returns a new data structure for a graph with v vertices and e edges */
Graph newGraph(int v, int e) {
    Graph g;
    g = (Graph)calloc(1, sizeof(struct _graph));
    if (g == NULL) {throw("newGraph");}

    g->vertexNumber = v;
    g->edgeNumber = e;
    g->initial = 0;
    g->vertexCounter = 1;
    return(g);
}

/* returns a new data structure for an automaton with v vertices and e edges */
Graph newAutomaton(int v, int e) {
    Graph aut;

    aut = newGraph(v, e);
    aut->target = (int *)calloc(e, sizeof(int));
    if (aut->target == NULL) {throw("newAutomaton");}
    aut->terminal = (int *)calloc(v, sizeof(int));
    if (aut->terminal == NULL) {throw("newAutomaton");}
    return(aut);
}

/* returns a new data structure for a suffix automaton with v vertices and e edges */
Graph newSuffixAutomaton(int v, int e) {
    Graph aut;

    aut = newAutomaton(v, e);
    for (int i = 0; i < e; i++) {
        aut->target[i] = UNDEFINED;
    }
    // memset(aut->target, UNDEFINED, e*sizeof(int));
    aut->suffixLink = (int *)calloc(v, sizeof(int));
    if (aut->suffixLink == NULL) {throw("newSuffixAutomaton");}
    aut->length = (int *)calloc(v, sizeof(int));
    if (aut->length == NULL) {throw("newSuffixAutomaton");}
    aut->position = (int *)calloc(v, sizeof(int));
    if (aut->position == NULL) {throw("newSuffixAutomaton");}
    aut->shift = (int *)calloc(e, sizeof(int));
    if (aut->shift == NULL) {throw("newSuffixAutomaton");}
    return(aut);
}


/* returns a new data structure for a trie with v vertices and e edges */
Graph newTrie(int v, int e) {
    Graph aut;

    aut = newAutomaton(v, e);
    for (int i = 0; i < e; i++) {
        aut->target[i] = UNDEFINED;
    }
    //    memset(aut->target, UNDEFINED, e*sizeof(int));
    aut->suffixLink = (int *)calloc(v, sizeof(int));
    if (aut->suffixLink == NULL) {throw("newTrie");}
    aut->length = (int *)calloc(v, sizeof(int));
    if (aut->length == NULL) {throw("newTrie");}
    aut->position = (int *)calloc(v, sizeof(int));
    if (aut->position == NULL){throw("newTrie");}
    aut->shift = (int *)calloc(e, sizeof(int));
    if (aut->shift == NULL) {throw("newTrie");}
    return(aut);
}


/* returns a new vertex for graph g */
int newVertex(Graph g) {
    if (g != NULL && g->vertexCounter <= g->vertexNumber) {
        return(g->vertexCounter++);}
    throw("newTrie");
}


/* returns the initial vertex of graph g */
int getInitial(Graph g) {
    if (g != NULL)
        {return(g->initial);}
    throw("getInitial");
}


/* returns true if vertex v is terminal in graph g */
boolean isTerminal(Graph g, int v) {
    if (g != NULL && g->terminal != NULL && v < g->vertexNumber)
        {return(g->terminal[v]);}
    throw("isTerminal");
}


/* set vertex v to be terminal in graph g */
void setTerminal(Graph g, int v) {
    if (g != NULL && g->terminal != NULL && v < g->vertexNumber)
        {g->terminal[v] = 1;}
    else
        {throw("setTerminal");}
}


/* returns the target of edge from vertex v labelled by character c in graph g */
int getTarget(Graph g, int v, unsigned char c) {
    if (g != NULL && g->target != NULL && v < g->vertexNumber && v*c < g->edgeNumber)
        {return(g->target[v*(g->edgeNumber/g->vertexNumber) +c]);}
    throw("getTarget");
}


/* add the edge from vertex v to vertex t labelled by character c in graph g */
void setTarget(Graph g, int v, unsigned char c, int t) {
    if (g != NULL && g->target != NULL && v < g->vertexNumber && v*c <= g->edgeNumber && t < g->vertexNumber)
        {g->target[v*(g->edgeNumber/g->vertexNumber) + c] = t;}
    else
        {throw("setTarget");}
}


/* returns the suffix link of vertex v in graph g */
int getSuffixLink(Graph g, int v) {
    if (g != NULL && g->suffixLink != NULL && v < g->vertexNumber)
        {return(g->suffixLink[v]);}
    throw("getSuffixLink");
}


/* set the suffix link of vertex v to vertex s in graph g */
void setSuffixLink(Graph g, int v, int s) {
    if (g != NULL && g->suffixLink != NULL && v < g->vertexNumber && s < g->vertexNumber)
        {g->suffixLink[v] = s;}
    else
        {throw("setSuffixLink");}
}


/* returns the length of vertex v in graph g */
int getLength(Graph g, int v) {
    if (g != NULL && g->length != NULL && v < g->vertexNumber)
        {return(g->length[v]);}
    throw("getLength");
}


/* set the length of vertex v to integer ell in graph g */
void setLength(Graph g, int v, int ell) {
    if (g != NULL && g->length != NULL && v < g->vertexNumber)
        {g->length[v] = ell;}
    else
        throw("setLength");
}


/* returns the position of vertex v in graph g */
int getPosition(Graph g, int v) {
    if (g != NULL && g->position != NULL && v < g->vertexNumber)
        {return(g->position[v]);}
    throw("getPosition");
}


/* set the length of vertex v to integer ell in graph g */
void setPosition(Graph g, int v, int p) {
    if (g != NULL && g->position != NULL && v < g->vertexNumber)
        {g->position[v] = p;}
    else
        {throw("setPosition");}
}


/* returns the shift of the edge from vertex v labelled by character c in graph g */
int getShift(Graph g, int v, unsigned char c) {
    if (g != NULL && g->shift != NULL && v < g->vertexNumber && v*c < g->edgeNumber)
        {return(g->shift[v*(g->edgeNumber/g->vertexNumber) + c]);}
    {throw("getShift");}
}


/* set the shift of the edge from vertex v labelled by character c to integer s in graph g */
void setShift(Graph g, int v, unsigned char c, int s) {
    if (g != NULL && g->shift != NULL && v < g->vertexNumber && v*c <= g->edgeNumber)
        {g->shift[v*(g->edgeNumber/g->vertexNumber) + c] = s;}
    else
        {throw("setShift");}
}


/* copies all the characteristics of vertex source to vertex target in graph g */
void copyVertex(Graph g, int target, int source) {
    if (g != NULL && target < g->vertexNumber && source < g->vertexNumber) {
        if (g->target != NULL){
            for (int i = 0; i < g->edgeNumber / g->vertexNumber; i++) {
                g->target[target * (g->edgeNumber / g->vertexNumber) + i] = g->target[source * (g->edgeNumber / g->vertexNumber) + i];
            }
        }
            // memcpy(g->target + target*(g->edgeNumber/g->vertexNumber),
            //     g->target + source*(g->edgeNumber/g->vertexNumber),
            //     (g->edgeNumber/g->vertexNumber)* sizeof(int));
        if (g->shift != NULL){
            for (int i = 0; i < g->edgeNumber / g->vertexNumber; i++) {
                g->shift[target * (g->edgeNumber / g->vertexNumber) + i] = g->shift[source * (g->edgeNumber / g->vertexNumber) + i];
            }
        }
            // memcpy(g->shift + target*(g->edgeNumber/g->vertexNumber), 
            //     g->shift + source*(g->edgeNumber/g->vertexNumber),
            //     (g->edgeNumber/g->vertexNumber)*sizeof(int));
        if (g->terminal != NULL)
            {g->terminal[target] = g->terminal[source];}
        if (g->suffixLink != NULL)
            {g->suffixLink[target] = g->suffixLink[source];}
        if (g->length != NULL)
            {g->length[target] = g->length[source];}
        if (g->position != NULL)
            {g->position[target] = g->position[source];}
    }
    else {throw("copyVertex");}
}

void buildSuffixAutomaton(char *x, int m, Graph aut) {
    int i, art, init, last, p, q, r;
    char c;

    init = getInitial(aut);
    art = newVertex(aut);
    setSuffixLink(aut, init, art);
    last = init;
    for (i = 0; i < m; ++i) {
        c = x[i];
        p = last;
        q = newVertex(aut);
        setLength(aut, q, getLength(aut, p) + 1);
        setPosition(aut, q, getPosition(aut, p) + 1);
        while (p != init && getTarget(aut, p, c) == UNDEFINED) {
            setTarget(aut, p, c, q);
            setShift(aut, p, c, getPosition(aut, q) - getPosition(aut, p) - 1);
            p = getSuffixLink(aut, p);
        }
        if (getTarget(aut, p, c) == UNDEFINED) {
            setTarget(aut, init, c, q);
            setShift(aut, init, c, getPosition(aut, q) - getPosition(aut, init) - 1);
            setSuffixLink(aut, q, init);
        }
        else
            if (getLength(aut, p) + 1 == getLength(aut, getTarget(aut, p, c))) {
                setSuffixLink(aut, q, getTarget(aut, p, c));
            }
            
            else {
                r = newVertex(aut);
                copyVertex(aut, r, getTarget(aut, p, c));
                setLength(aut, r, getLength(aut, p) + 1);
                setSuffixLink(aut, getTarget(aut, p, c), r);
                setSuffixLink(aut, q, r);
                while (p != art && getLength(aut, getTarget(aut, p, c)) >= getLength(aut, r)) {
                    setShift(aut, p, c, getPosition(aut, getTarget(aut, p, c)) -getPosition(aut, p) - 1);
                    setTarget(aut, p, c, r);
                    p = getSuffixLink(aut, p);
                }
            }
        last = q;
    }
    setTerminal(aut, last);
    while (last != init) {
        last = getSuffixLink(aut, last);
        setTerminal(aut, last);
    }
}

void OUTPUT(int book_code, int page, int paragraph, int sentence_no, int k, Node* &curr, int&n_matches, unsigned long&currCharCount) {
    curr->right = new Node(book_code, page, paragraph, sentence_no, (k+currCharCount));
    curr->right->left = curr;
    curr = curr->right;
    n_matches++;
}

#define ASIZE 256

void FDM(int book_code, int page, int paragraph, int sentence_no, string &text, string &pattern, Node* &curr, int&n_matches, unsigned long&currCharCount) {
    cout << "hello1\n";
    char t0 = text[0]; char p0 = pattern[0];
    char*x = &p0; char*y = &t0;
    int m = pattern.length(); int n = text.length();

    int j, init, ell, state;
    Graph aut;

   /* Preprocessing */
    aut = newSuffixAutomaton(2*(m + 2), 2*(m + 2)*ASIZE);
    buildSuffixAutomaton(x, m, aut);
    init = getInitial(aut);

    cout << "hello2\n";
    /* Searching */
    ell = 0;
    state = init;
    for (int j = 0; j < n; ++j) {
        cout << "hello3\n";
        if (getTarget(aut, state, y[j]) != UNDEFINED) {
            cout << "hello4\n";
            ++ell;
            state = getTarget(aut, state, y[j]);
        }
        else {
            cout << "hello5\n";
            while (state != init && getTarget(aut, state, y[j]) == UNDEFINED) {
                cout << "hello6\n";
                state = getSuffixLink(aut, state);
                if (getTarget(aut, state, y[j]) != UNDEFINED) {
                    ell = getLength(aut, state) + 1;
                    state = getTarget(aut, state, y[j]);
                }
                else {
                    ell = 0;
                    state = init;
                }
                cout << "hello7\n";
            }   
            cout << "hello8\n";             
        }
        cout << "hello9\n";
        if (ell == m) {
            // cout << j - m + 1 << "\n";
            cout << "hello10\n";
            OUTPUT(book_code, page, paragraph, sentence_no, j - m + 1, curr, n_matches, currCharCount); 
            cout << "hello11\n";
        }
        cout << "hello12\n";
    }
    cout << "hello13\n";
}



//=========================================================

SearchEngine::SearchEngine(){
    // Implement your function here  
    

}

SearchEngine::~SearchEngine(){
    // Implement your function here  
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){ //checked, works
    // Implement your function here  
    allStringsInfo.push_back(Info(book_code, page, paragraph, sentence_no, sentence)); 
    //processing, ie, storing. This function will be called for all sentences before search is used
    return;
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here  
    Node* head = new Node();
    Node* curr = head;
    unsigned long currCharCount = 0;

    for (int i = 0; i < allStringsInfo.size(); i++) {

        FDM(allStringsInfo[i].a, allStringsInfo[i].b, allStringsInfo[i].c, allStringsInfo[i].d, allStringsInfo[i].s, pattern, curr, n_matches, currCharCount);
        cout << "FDM DONE\n";
        currCharCount += allStringsInfo[i].s.length();
        cout << currCharCount << "\n";
        // cout << curr->left->offset << "\n";
    }

    if (n_matches > 0) {
        return head;
    } else {
        return nullptr;
    }    
}

int main () {

    SearchEngine SE;

    SE.insert_sentence(1, 1, 1, 1, "Hello! This is a test sentence.");
    SE.insert_sentence(1, 1, 2, 1, "Hello! This is another test sentence.");

    for (Info x: SE.allStringsInfo) {
        cout << x.a << ", " << x.b << ", " << x.c << ", " << x.d << ", " << x.s << "\n";
    }

    int x = 0;
    
    Node* result = SE.search("test", x);

    cout <<(result == NULL) << "\n";

    return 0;
}