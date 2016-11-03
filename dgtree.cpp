#include <queue>
#include "dgtree.h"

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


void treeGrow(DGTreeNode *root) {
    // Heap of possible child nodes of root ordered by score for root->fgraph
    priority_queue<DGTreeNode *> *H; 
 
    //cout <<"Total Score "<< score(root) << endl;
          
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
