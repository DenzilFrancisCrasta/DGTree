#include "query.h"
#include <iostream>
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

       if(q == NULL){
         //shouldn't come here --> indicates missing links/graphs in the index
         cout<<"The index didn't cover all data graphs\n";
         exit(1);
       } // analyse carefully and complete
       else {
         DGTreeNode *g = q->treenode;
         /*for(every g_plus in g->children){ //iterate over g->children
           if (g_plus has no children) // if(g_plus->children.size() ==0){
             if(grow_edge == NULL) {//check for NULL grow_edge
                //A = A U gplus_S (The graph already matched its parent, with NULL grow_edge, it definitely matches this, so add to answer set
             } 
             else { //There is an edge to grow
               //check to extend each match; iterate over all matches in q
               //similar to FeatureExpansion logic
               //... paper
             } //end if-else of there is an edge to grow
           } 
           else { //g_plus has children
             FeatureExpansion(Q,q,g_plus,H,C); //Put all children on heap with scores
           }

         }*/ //end iterate over all children
       }
       
    }
    
     
}

bool compareMapEntryNodes(const pair<int, Graph *>& p1,const pair<int, Graph *>& p2) {
    return p1.first < p2.first;
}

void FeatureExpansion(Graph *Q, Entry *q, DGTreeNode *g_plus, Q_Heap *H, map<int, Graph *> *C){
   Entry *q_plus = new Entry;
   q_plus->treenode = g_plus;
   //TO_DO: q_plus->S_star = g_plus->S intersection C;
   q_plus->matches = NULL;
   if(g_plus->grow_edge == NULL){ //The map in parent is sufficient
      //add to answer set or return NULL; will it come here?
      //this code cannot be reached since grow_edge is NULL only in leaf nodes, which gets checked in the if part, else part calls this FeatureExpansion function.
   }
   else {
      int ui = g_plus->grow_edge->x;
      int uj = g_plus->grow_edge->y;
      int valence = g_plus->grow_edge->valence;
      
      /*for(each match that has the correct ui label){ //iterate over all matches, if ui label is incorrect, continue
         if(g_plus->edge_type == OPEN){
            //for every neighbour v of f(ui) in Q {
                if label of v matches label of uj {
                   add this match to q_plus->matches
                }
            }
         } //end edge_type OPEN case
         else { // edge_type == CLOSE
            //paper
         }
     } // end for each valid match*/
   }
   
  //paper  if-else same 
}
Entry * BestFeature(Q_Heap *H, map<int, Graph *> *C ){
    Entry *q = new Entry;
    q = H->top();

    while (! includes(C->begin(), C->end(), q->S_star->begin(), q->S_star->end(), compareMapEntryNodes)) {
       map<int, Graph *> *new_S_star = new map<int, Graph *>();

       set_intersection(q->S_star->begin(), q->S_star->end(), C->begin(), C->end(), insert_iterator<map<int, Graph *> >(*new_S_star, new_S_star->begin()));
       delete q->S_star;
       q->S_star = new_S_star;

       H->pop(); //possibly causes segmentation fault on empty H
       if (!q->S_star->empty()) {
            q->score = score(q);
	    H->push(q);
       }

       q = H->top(); //possibly causes segmentation fault on empty H

    } // end while g+.S* is not included in C 

    return q; 
}

int main() {
    return 0;
}
