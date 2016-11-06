#include "query.h"
#include <iostream>

float score(Entry *node) {  
   
    float covered_datagraphs;
    float total_matches = node->matches->size(); 

    covered_datagraphs = node->S_star->size();
   //cout << "Total Matches" << total_matches << endl;

   float score = covered_datagraphs / total_matches;
   
   return score;
}

/*
bool isAnEdge(adj_list_t *adj_list, int u, int v) {
    
    // u is larger than the size of the list 
    if (u >= adj_list->size())
        return false;
    
    // get the edge list of u
    edge_list& elist = (*(adj_list))[u];
    
    for (edge_list_itr elist_itr = elist.begin(); elist_itr != elist.end(); elist_itr++) {
        if (elist_itr->first == v)
            return true;
    }    

    return false;
}
*/

map<int, Graph *> *search(DGTreeNode *root, Graph *Q) {

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


    map<int, Graph *> *A = new map<int, Graph *>();
    Q_Heap *H = new Q_Heap(); 

    Entry *q = new Entry;
    q->treenode = root;
    q->S_star = C;
    q->matches = new list<vector<int> *>();
    
    //for every vertex in query graph, add a match f with the vertex number
    for(int i=0; i<Q->vertex_count; i++){
       vector<int> *f = new vector<int>(1);
       f->at(0) = i;
       (*(q->matches)).push_back(f);
     
    } // end for every vertex of query graph

    q->score = score(q);
    H->push(q);
    
    int count=0;
    while(!C->empty() && count<300){

       count++;
       q = BestFeature(H,C);
      
       //cout << "C size at loop start" << C->size() << endl;
       if(q == NULL){
         //shouldn't come here --> indicates missing links/graphs in the index
         cerr<<"The index didn't cover all data graphs\n";
         exit(1);
       } // analyse carefully and complete
       else {

         DGTreeNode *g = q->treenode;
         vector<DGTreeNode *>::const_iterator itr = g->children.begin();

         // for every child node of g
         while (itr != g->children.end()) {
             DGTreeNode *g_plus;
             g_plus = *itr;
             
             if (g_plus->children.size() ==0) { 

                 // g_plus is a leaf
                 if (g_plus->grow_edge == NULL) {

                    //A = A U gplus_S
                    map<int, Graph *> *newA = new map<int, Graph *>();
                    set_union(A->begin(), A->end(), g_plus->S->begin(), g_plus->S->end(), insert_iterator<map<int, Graph *> >(*newA, newA->begin())); 
                    //delete A;
                    A = newA;
                    
                             map<int, Graph *> *D = new map<int, Graph *>();    
                            set_difference(C->begin(), C->end(), g_plus->S->begin(), g_plus->S->end(), insert_iterator<map<int, Graph *> >(*D, D->begin())); 
                            C = D; 



                 } else {
                        
                    // check to extend each match; iterate over all matches in q
                    // similar to FeatureExpansion logic
                    // paper


                      int ui = g_plus->grow_edge->x;
                      int uj = g_plus->grow_edge->y;
                      string gxlabel = g_plus->grow_edge->x_label;
                      string gylabel = g_plus->grow_edge->y_label;
                      int valence = g_plus->grow_edge->valence;


                      list<vector<int> *>::const_iterator mitr = q->matches->begin(); 
                      while (mitr != q->matches->end()) {
                      
                          bool new_match_found = false;

                          int q_index = (**mitr)[ui];
                          string uilabel = Q->vertex_labels[q_index];

                          if (uilabel == gxlabel) {
                            // correct ui label ie valid match  
                             if ( g_plus->edge_type == OPEN) {

                              // get the edge list of the q_index in Q
                              edge_list& elist = (*(Q->adjacencyList))[q_index];

                              //for every neighbor of q_index in Q 
                              for (edge_list_itr elist_itr = elist.begin(); elist_itr != elist.end(); elist_itr++) {

                                  node_index_t v = elist_itr->first;
                                  string vlabel = Q->vertex_labels[v];
                                  int Q_valence = elist_itr->second;

                                  if (Q_valence == valence && vlabel == gylabel) {
                                       if (find((**mitr).begin(), (**mitr).end(), v) == (**mitr).end()) {
                                        
                                            new_match_found = true;
                                            break ; 
                                       }
                                  }

                              }//endfor every neighbor of 


                                   
                             } // end if edge type open
                             else {
                                 // closed edge case 

                                 if (isAnEdge(Q->adjacencyList, (**mitr)[ui], (**mitr)[uj])) {
                                            new_match_found = true;
                                 
                                  }// end if is an edge
                         
                             } //end else closed edge case
                             
                             // Add an element to the answer set
                             if (new_match_found) {
                                map<int, Graph *> *newA = new map<int, Graph *>();
                                set_union(A->begin(), A->end(), g_plus->S->begin(), g_plus->S->end(), insert_iterator<map<int, Graph *> >(*newA, newA->begin())); 
                                //delete A;
                                A = newA;
                             }


                          //  cout << "gplus size at loop end" << g_plus->S->size() << endl;
                            map<int, Graph *> *D = new map<int, Graph *>();    
                            set_difference(C->begin(), C->end(), g_plus->S->begin(), g_plus->S->end(), insert_iterator<map<int, Graph *> >(*D, D->begin())); 
                            C = D; 


                      }// end if uilabel = gxlabel
                  
                      mitr++;
                  } // end while 

                 }
             }
             else {
                  
                 FeatureExpansion(Q, q, g_plus, H, C); //Put all children on heap with scores
              //   cout << "gplus size after feature expansion" << g_plus->S->size() << endl;
             }

             itr++; 
         } // end for every child node of g

       }
       
 //      cout << "C size at loop end" << C->size() << endl;
    }
    
   return A;     
}

bool compareMapEntryNodes(const pair<int, Graph *>& p1,const pair<int, Graph *>& p2) {
    return p1.first < p2.first;
}


void FeatureExpansion(Graph *Q, Entry *q, DGTreeNode *g_plus, Q_Heap *H, map<int, Graph *> *C){

   Entry *q_plus = new Entry;
   q_plus->treenode = g_plus;
   q_plus->S_star = new map<int, Graph *>();
   q_plus->matches = new list<vector<int> *>();

   //: q_plus->S_star = g_plus->S intersection C;

       set_intersection(g_plus->S->begin(), g_plus->S->end(), C->begin(), C->end(), insert_iterator<map<int, Graph *> >(*(q_plus->S_star), q_plus->S_star->begin()));

   //q_plus->matches = NULL;
   if(g_plus->grow_edge == NULL){
      // The map in parent is sufficient
      // add to answer set or return NULL; will it come here?
      // this code cannot be reached since grow_edge is NULL only in leaf nodes, which gets checked in the if part, else part calls this FeatureExpansion function.
       cerr << "We shouldnt reach here. Houston we have a problem" << endl;
       exit(1);
   }
   else {
      int ui = g_plus->grow_edge->x;
      int uj = g_plus->grow_edge->y;
      string gxlabel = g_plus->grow_edge->x_label;
      string gylabel = g_plus->grow_edge->y_label;
      int valence = g_plus->grow_edge->valence;

      list<vector<int> *>::const_iterator itr = q->matches->begin(); 
      while (itr != q->matches->end()) {
          int q_index = (**itr)[ui];
          string uilabel = Q->vertex_labels[q_index];

          if (uilabel == gxlabel) {
            // correct ui label ie valid match  
             if ( g_plus->edge_type == OPEN) {

              // get the edge list of the q_index in Q
              edge_list& elist = (*(Q->adjacencyList))[q_index];

              //for every neighbor of q_index in Q 
              for (edge_list_itr elist_itr = elist.begin(); elist_itr != elist.end(); elist_itr++) {

                  node_index_t v = elist_itr->first;
                  string vlabel = Q->vertex_labels[v];
                  int Q_valence = elist_itr->second;

                  if (Q_valence == valence && vlabel == gylabel) {
                       if (find((**itr).begin(), (**itr).end(), v) == (**itr).end()) {
                          
                             vector<int> *m = new vector<int>(**itr); 
                             m->push_back(v);
                             (*(q_plus->matches)).push_back(m); 
                       
                       }
                  
                  }


              }//endfor every neighbor of 
                   
             } // end if edge type open
             else {
                 // closed edge case 

                 if (isAnEdge(Q->adjacencyList, (**itr)[ui], (**itr)[uj])) {
                             vector<int> *m = new vector<int>(**itr); 
                             (*(q_plus->matches)).push_back(m); 

                 
                 }// end if is an edge
             
             } //end else closed edge case
          }// end if uilabel = gxlabel
      
          itr++;
      } // end while 


      
      /*
             for(each match that has the correct ui label){ //iterate over all matches, if ui label is incorrect, continue
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
              } // end for each valid match
           
       */


   } 

  if (! q_plus->matches->empty()) {
     q_plus->score = score(q_plus); 
     H->push(q_plus);

  } else {

    map<int, Graph *> *D = new map<int, Graph *>();    
    set_difference(C->begin(), C->end(), q_plus->S_star->begin(), q_plus->S_star->end(), insert_iterator<map<int, Graph *> >(*D, D->begin())); 
    C = D; 
  
  }
   
}

Entry * BestFeature(Q_Heap *H, map<int, Graph *> *C ){
    Entry *q = new Entry;
    q = H->top();
    H->pop();
    while (! includes(C->begin(), C->end(), q->S_star->begin(), q->S_star->end(), compareMapEntryNodes)) {
       map<int, Graph *> *new_S_star = new map<int, Graph *>();

       set_intersection(q->S_star->begin(), q->S_star->end(), C->begin(), C->end(), insert_iterator<map<int, Graph *> >(*new_S_star, new_S_star->begin()));
       //delete q->S_star;
       q->S_star = new_S_star;

       //H->pop(); //possibly causes segmentation fault on empty H
       if (!q->S_star->empty()) {
            q->score = score(q);
	    H->push(q);
       }

       q = H->top(); //possibly causes segmentation fault on empty H
       H->pop();

    } // end while g+.S* is not included in C 

    return q; 
}

