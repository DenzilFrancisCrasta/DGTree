#include "query.h"

vector<int> *search(DGTreeNode *root, Graph *Q) {

    map<int, Graph *> *C = new map<int, Graph *>(); // candidate answers     

    map<int, Graph *> *S = root->S;
    map<int, Graph *>::const_iterator itr = S->begin();

    // sieve data grpahs which can become subgraphs of Q
    while (itr != S->end()) {

        Graph *g = itr->second;    

        if (g->edge_count <= Q->edge_count && g->vertex_count <= Q->vertex_count) {
            C[g->id] = g ;
        }

        itr++;
    }// end while sieve data graphs


    vector<int> *A = new vector<int>();
    Q_Heap *H = new Q_Heap(); 

    Entry *q = new Entry;
    q->treenode = root;
    q->S = C;
    








    
     
}

int main() {
    return 0;
}
