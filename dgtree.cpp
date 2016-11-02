#include "dgtree.h"


DGTreeNode *DGTreeConstruct(map<int, Graph *>& data_graphs) {
   DGTreeNode *root;
   int graph_id;
   Graph *G;
   vector<int> *f;

   // make a new DGTree Node 
   root = new DGTreeNode;
   //REMEMBER: fgraph is POINTER to adjacency list 
   root->fgraph = new vector<list<pair<int, int> > >(1);

   root->S = data_graphs; 
   root->S_star = data_graphs;
   root->grow_edge = NULL;
   root->edge_type = OPEN;

   map<int, Graph *>::const_iterator itr = data_graphs.begin();

   // For all data graphs create the set of matches.
   // Initially all matches will have single vertex
   while (itr != data_graphs.end()) {
 
      // get graph id and data-graph from the map
      graph_id = itr->first;
      G = itr->second; // a data graph

      // for every vertex of data-graph
      for (int i=0; i<G->vertex_count; i++) {
          f = new vector<int>(1);
          f->at(0) = i;
          root->matches_of[graph_id].push_back(f);
      } // end for every vertex of data graph

      itr++; 
   }// end while for every data graph

  
   TreeGrow(root);

   return root;
}


void TreeGrow(DGTreeNode *root) {
       
}
