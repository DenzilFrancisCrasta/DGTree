#include <map>
#include "graph.h"

using namespace std;

struct edge {
   // represents an edge between x and y
   int x; 
   int y;
   int valence;
};

enum e_type {OPEN, CLOSE}; 

struct DGTreeNode {
  /* set of child tree nodes */
  vector<DGTreeNode *>           children; 
  /* feature graph for the current node */
  vector<list<pair<int, int> > > *fgraph; 
  /* the edge used to grow the feature graph of parent node*/
  edge                           *grow_edge;  //
  e_type                         edge_type;  
  map<int, Graph *>              S; // set of data graphs which cover the feature graph
  map<int, Graph *>              S_star; // subset of data graphs which cover the feature graph
  map<int, list<vector<int> *> >  matches_of;

  float                          score;  // score of fgraph 
};


DGTreeNode *DGTreeConstruct(map<int, Graph *>& data_graphs);
void TreeGrow(DGTreeNode *root);

   
