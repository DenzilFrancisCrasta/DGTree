#include "dgtree.h"
#include <iostream>
#include <algorithm>

/*
 * Set a bias score to increase the score of a feature graph that provides
 * optimum balance between cost sharing during query processing  and 
 * pruning power. 
 * Paper suggests to use #datagraphs as a good value 
 */
float BIAS_SCORE;

DGTreeNode *DGTreeConstruct(map<int, Graph *> *data_graphs) {
   DGTreeNode *root;
   int graph_id;
   Graph *G;
   vector<int> *f;

   BIAS_SCORE = data_graphs->size();

   // make a new DGTree Node 
   root = new DGTreeNode;
   //REMEMBER: fgraph is POINTER to adjacency list 
   root->fgraph = new vector<list<pair<int, int> > >(1);

   root->S = data_graphs; 
   root->S_star = data_graphs;
   root->matches_of = new map<int, list<vector<int> *> >();
   root->grow_edge = NULL;
   root->vertex_labels = NULL;
   root->edge_type = OPEN;

   map<int, Graph *>::const_iterator itr = data_graphs->begin();

   // For all data graphs create the set of matches.
   // Initially all matches will have single vertex
   while (itr != data_graphs->end()) {
 
      // get graph id and data-graph from the map
      graph_id = itr->first;
      G = itr->second; // a data graph

      // for every vertex of data-graph
      for (int i=0; i<G->vertex_count; i++) {
          f = new vector<int>(1);
          f->at(0) = i;
          (*(root->matches_of))[graph_id].push_back(f);
      } // end for every vertex of data graph

      itr++; 
   }// end while for every data graph

  
   treeGrow(root);

   return root;
}

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

void printAndDestroyHeap(DG_Heap *H) {
    while (!H->empty()) {
       DGTreeNode *p = H->top(); 
       cout << p->grow_edge->x_label << "-" 
	    << p->grow_edge->valence << "-" 
	    <<p->grow_edge->y_label 
	    << " [g.S=" << p->S->size() << "]"<<" [Score " << p->score << "]"<<endl;
       H->pop();
    }
}

void treeGrow(DGTreeNode *root) {
    // Heap of possible child nodes of root ordered by score for root->fgraph
    DG_Heap *H; 
    DGTreeNode *g_plus;

    H = candidateFeatures(root); 

    map<int, Graph *> C(*(root->S_star)); //data-graphs to be covered    
   
    
    while (!C.empty()) {
        g_plus = bestFeature(H, &C);
        
        if (g_plus->S_star->size() > 1) {

            
            // add the edge  (ui,uj) with valence to the feature graph of g_plus
            int ui = g_plus->grow_edge->x;
            int uj = g_plus->grow_edge->y;
            int valence = g_plus->grow_edge->valence;

            // allocate storage in the feature graph for the open edge
            if (g_plus->edge_type == OPEN) {
                g_plus->fgraph->push_back(list<pair<int, int> >());
            }
  
            (*(g_plus->fgraph))[ui].push_back(make_pair(uj, valence));
            (*(g_plus->fgraph))[uj].push_back(make_pair(ui, valence));
  
            treeGrow(g_plus);
        
        } else {
            // we have reached a leaf node which holds a data-graph   
            
            if (root->S_star->size() == 1) {
              g_plus->fgraph = root->S_star->begin()->second->adjacencyList;   
              g_plus->S = g_plus->S_star;
            } else {
              cerr << "ERROR: Expected S* to have one graph but size(S*) = " << root->S_star->size() << endl; 
            }

        }// end reached a leaf node 
        

        map<int, Graph *> D;    
        set_difference(C.begin(), C.end(), g_plus->S_star->begin(), g_plus->S_star->end(), insert_iterator<map<int, Graph *> >(D, D.begin())); 
	C = D; //costly find alternative
    
    }// end while data-graphs to be covered is not empty
   

   // cout << "Candidate Features size " << H->size()<< endl;
   // printAndDestroyHeap(H);
}

bool compareMapDGTreeNodes(const pair<int, Graph *>& p1, const pair<int, Graph *>& p2) {
    return p1.first < p1.second;
}

DGTreeNode *bestFeature(DG_Heap *H, map<int, Graph *> *C) {
    DGTreeNode *g_plus = H->top();
    while (! includes(C->begin(), C->end(), H->begin(), H->end(), compareMapDGTreeNodes)) {
       map<int, Graph *> *new_S_star = new map<int, Graph *>();

       set_intersection(g_plus->S_star->begin(), g_plus->S_star->end(), C->begin(), C->end(), insert_iterator<map<int, Graph *> >(*new_S_star, new_S_star->begin()));
       delete g_plus->S_star;
       g_plus->S_star = new_S_star;

       H->pop();
       if (!g_plus->S_star->empty()) {
            g_plus->score = score(g_plus);
	    H->push(g_plus);
       }

       g_plus = H->top();

    } // end while g+.S* is not included in C 
    
    return g_plus;
}

edge *makeEdge(int ui, int uj, int valence, string x_label, string y_label) {
    edge *e = new edge;
    e->x = ui;
    e->y = uj;
    e->valence = valence; 
    e->x_label = x_label; 
    e->y_label = y_label;  
    
    return e;
}

DG_Heap *candidateFeatures(DGTreeNode *node) {
   // a new heap to store DGTreeNodes
   DG_Heap *H = new DG_Heap(); 
   map<edge, DGTreeNode *, compare_edges> H_map; // stores the possible children of node for fast retrieval by an edge

   int graph_id;
   int valence;
   DGTreeNode *g_plus; 

   /**********************************  PART 1 ********************************/
   // for every graph in node->S_star
   for (const_mp_itr itr = node->S_star->begin(); itr != node->S_star->end(); itr++) {

       //get G's graph id and the list of matches of feature graph with G
       graph_id = itr->first;
       Graph *G = itr->second;
       matches_list g_match_list =  node->matches_of->at(graph_id);

       // for every match f of feature graph in node in G 
       for (const_list_itr f = g_match_list.begin(); f != g_match_list.end(); f++) {
           int f_size = (*f)->size();
        
           //for every node in the feature graph
           for (node_index_t ui=0; ui < f_size ; ui++) {
               
              // find the index of the match of ui in G
              node_index_t w = (**f)[ui];   

              // get the edge list of the G_node_index in G
              edge_list& elist = (*(G->adjacencyList))[w];

              //for every neighbor of f(ui) in G 
              for (edge_list_itr elist_itr = elist.begin(); elist_itr != elist.end(); elist_itr++) {
                  node_index_t v = elist_itr->first;
		  valence = elist_itr->second;
                  vector<int>& match = (**f);
                  vector<int>::iterator uj_position;

                  node_index_t uj;
                  e_type t;

                  if ((uj_position =find(match.begin(), match.end(), v)) != match.end()) {
                     // check if v is already in the match f
                     t = CLOSE; //edge type would be CLOSE
                     uj = uj_position - match.begin(); // get the index of f_inverse(v) ie uj in the match f
                  } else {
                     // v is a new vertex to be mapped in f 
                     t = OPEN; //edge type would be OPEN
                     uj = f_size;  //uj is given index as |f| (zero based indexing) since it is a new node being added
                  }


                  if ( uj > ui  && !isAnEdge(node->fgraph, ui, uj)) {
		      //construct the key to search 
		      edge *e = makeEdge(ui, uj, valence, G->vertex_labels[w],G->vertex_labels[v]); 
		      // g+ = H.find((ui, uj));
		      map<edge, DGTreeNode *, compare_edges>::iterator g_plus_itr = H_map.find(*e);
		      
		      // g+ = empty ie no such dgtreenode is found
		      if (g_plus_itr == H_map.end()) {
			  g_plus = new DGTreeNode;

                          g_plus->grow_edge = e;

                         
                          if (node->vertex_labels == NULL) {
                              // If we are at level 1 the root does not have vertex labels
                              g_plus->vertex_labels = new vector<string>(); 
                              g_plus->vertex_labels->push_back(G->vertex_labels[w]);
                              g_plus->vertex_labels->push_back(G->vertex_labels[v]);
                              g_plus->fgraph = new vector<list<pair<int, int> > >(2);

                          
                          } else {
                              g_plus->vertex_labels = new vector<string>(*(node->vertex_labels)); 
                              g_plus->vertex_labels->push_back(G->vertex_labels[v]);
                          }

			  g_plus->score = 0;
			  g_plus->edge_type = t;

			  //add graph G to the set S_star of g_plus
			  g_plus->S_star = new map<int, Graph *>();
			  g_plus->S = new map<int, Graph *>();
                          g_plus->matches_of = new map<int, list<vector<int> *> >();
			  (*(g_plus->S_star))[graph_id] = G;  

		          //H->push(g_plus); // defer till part 2 completes as H_map will have the values which will help in building H	
			  H_map[*e] = g_plus; 
		      
		      } 
		      else {
                          //g+ is not empty. Just add G to g+.S_star
		          (*(g_plus_itr->second->S_star))[graph_id] = G;  
		      }
                  }// end if we have to add the edge ui,uj
              } // end for every neighbor of f(ui) in G 
           } //end for every node in the feature graph
       } // end for every match of feature graph of node in G
   }// end for every graph in node->S_star


   /********************************** PART 2 *****************************/

   // for every graph in node->S
   for (const_mp_itr itr = node->S->begin(); itr != node->S->end(); itr++) {

       //get G's graph id and the list of matches of feature graph with G
       graph_id = itr->first;
       Graph *G = itr->second;
       matches_list g_match_list =  node->matches_of->at(graph_id);

       // for every match f of feature graph in node in G 
       for (const_list_itr f = g_match_list.begin(); f != g_match_list.end(); f++) {
           int f_size = (*f)->size();
        
           //for every node in the feature graph
           for (node_index_t ui=0; ui < f_size ; ui++) {
               
              // find the index of the match of ui in G
              node_index_t w = (**f)[ui];   

              // get the edge list of the G_node_index in G
              edge_list& elist = (*(G->adjacencyList))[w];

              //for every neighbor of f(ui) in G 
              for (edge_list_itr elist_itr = elist.begin(); elist_itr != elist.end(); elist_itr++) {
                  node_index_t v = elist_itr->first;
		  valence = elist_itr->second;
                  vector<int>& match = (**f);
                  vector<int>::iterator uj_position;

                  node_index_t uj;
                  e_type t;

                  if ((uj_position =find(match.begin(), match.end(), v)) != match.end()) {
                     // check if v is already in the match f
                     uj = uj_position - match.begin(); // get the index of f_inverse(v) ie uj in the match f
                  } else {
                     // v is a new vertex to be mapped in f 
                     uj = f_size; //uj is given index as |f| (zero based indexing)  since it is a new node being added
                  }


                  if ( uj > ui  && !isAnEdge(node->fgraph, ui, uj)) {
		      //construct the key to search 
		      edge *e = makeEdge(ui, uj, valence, G->vertex_labels[w],G->vertex_labels[v]); 
		      // g+ = H.find((ui, uj));
		      map<edge, DGTreeNode *, compare_edges>::iterator g_plus_itr = H_map.find(*e);
		      
		      // g+ = empty ie no such dgtreenode is found
		      if (g_plus_itr != H_map.end()) {

			  g_plus = g_plus_itr->second;
			  (*(g_plus->S))[graph_id] = G;  

			  if (g_plus->edge_type == OPEN) {
                                
                             vector<int> *m = new vector<int>(**f); 
                             m->push_back(v);
                             (*(g_plus->matches_of))[graph_id].push_back(m); 
			  
			  } else {

                             vector<int> *m = new vector<int>(**f); 
                             (*(g_plus->matches_of))[graph_id].push_back(m); 
			  }

		         // H->push(g_plus);	
			 // H_map[*e] = g_plus; 
		      
		      } 
                  }// end if we have to add the edge ui,uj
              } // end for every neighbor of f(ui) in G 
           } //end for every node in the feature graph
       } // end for every match of feature graph of node in G
   }// end for every graph in node->S


  // Build H from HMAP after updating g+ scores  
   map<edge, DGTreeNode *, compare_edges>::iterator itr = H_map.begin();
   while (itr != H_map.end()) {
       DGTreeNode *n; 
       n = itr->second;
       n->score = score(n);
       H->push(n);
       itr++; 
   }
  
   return H; // return the Heap of possible child DGTreenodes of node    
}

float score(DGTreeNode *node) {  
   
    float covered_datagraphs; // count of datagraphs covered by the feature graph of node
    float datagraphs_containing_fgraph; // count of datagraphs containing the feature graph of node
    float total_matches; // total count of matches of feature graph of node with data-graphs
    float avg_matches; // avg matches of feature graph of node with data-graphs

    
    covered_datagraphs = node->S_star->size();
    datagraphs_containing_fgraph = node->S->size(); 

    // PERFORMANCE UPGRADE : cache this value 
    total_matches = 0;
    map<int, list<vector<int> *> >::const_iterator itr = node->matches_of->begin();

    while (itr != node->matches_of->end()) {
	total_matches += itr->second.size();
        itr++; 
    }

   //cout << "Total Matches" << total_matches << endl;

   avg_matches = total_matches / datagraphs_containing_fgraph ;
   float score = covered_datagraphs / avg_matches ;
   
   // CLOSED edge is an edge between two vertices that are already matched
   // we need to assign a higher score to this feature graph so we increase
   // score by a BIASED value

   score = (node->edge_type == CLOSE)? score+BIAS_SCORE : score;

   return score;
}
