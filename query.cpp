#include "query.h"
Entry * BestFeature(Q_Heap *H, map<int, Graph *> *C );
float score(Entry *node) {  
   
    float covered_datagraphs;
    float total_matches = node->matches->size(); 

    covered_datagraphs = node->S_star->size();
   //cout << "Total Matches" << total_matches << endl;

   float score = covered_datagraphs / total_matches;
   
   return score;
}
vector<int> *search(DGTreeNode *root, Graph *Q) {

    map<int, Graph *> *C = new map<int, Graph *>(); // candidate answers     

    map<int, Graph *> *S = root->S;
    map<int, Graph *>::const_iterator itr = S->begin();

    // sieve data grpahs which can become subgraphs of Q
    while (itr != S->end()) {

        Graph *g = itr->second;    

        if (g->edge_count <= Q->edge_count && g->vertex_count <= Q->vertex_count) {
           (*C)[g->id] = g ;
        }

        itr++;
    }// end while sieve data graphs


    vector<int> *A = new vector<int>();
    Q_Heap *H = new Q_Heap(); 

    Entry *q = new Entry;
    q->treenode = root;
    q->S_star = C;
    q->matches = NULL;
    
    //for every vertex in query graph, add a match f with the vertex number
    for(int i=0; i<Q->vertex_count; i++){
       vector<int> *f = new vector<int>(1);
       f->at(0) = i;
       (*(q->matches)).push_back(f);
     
    } // end for every vertex of query graph

    q->score = score(q);
    H->push(q);

    while(!C->empty()){
       q = BestFeature(H,C);

       if(q == NULL){} // analyse carefully and complete
       else {
          
       }
       
    }
    
     
}

bool compareMapEntryNodes(const pair<int, Graph *>& p1,const pair<int, Graph *>& p2) {
    return p1.first < p2.first;
}

Entry * BestFeature(Q_Heap *H, map<int, Graph *> *C ){
    Entry *q = new Entry;
    q = H->top();

    while (! includes(C->begin(), C->end(), q->S_star->begin(), q->S_star->end(), compareMapEntryNodes)) {
       map<int, Graph *> *new_S_star = new map<int, Graph *>();

       set_intersection(q->S_star->begin(), q->S_star->end(), C->begin(), C->end(), insert_iterator<map<int, Graph *> >(*new_S_star, new_S_star->begin()));
       delete q->S_star;
       q->S_star = new_S_star;

       H->pop();
       if (!q->S_star->empty()) {
            q->score = score(q);
	    H->push(q);
       }

       q = H->top();

    } // end while g+.S* is not included in C 

    return q; 
}

int main() {
    return 0;
}
