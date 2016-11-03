#include <map>
#include <queue>
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
  vector<string> *vertex_labels; // labels of vertex of feature graph
  /* feature graph for the current node */
  vector<list<pair<int, int> > > *fgraph; 
  /* the edge used to grow the feature graph of parent node*/
  edge                           *grow_edge;  //
  e_type                         edge_type;  
  map<int, Graph *>              *S; // set of data graphs which cover the feature graph
  map<int, Graph *>              *S_star; // subset of data graphs which cover the feature graph
  map<int, list<vector<int> *> > *matches_of;

  float                          score;  // score of fgraph 
};

struct compare_nodes {
  bool operator()(DGTreeNode *node_a, DGTreeNode *node_b) {
                  return node_a->score < node_b->score;
  }
};

typedef map<int, Graph *>::const_iterator const_mp_itr;
typedef priority_queue<DGTreeNode *, vector<DGTreeNode *>, compare_nodes> DG_Heap; 
typedef list<vector<int> *>::const_iterator const_list_itr;
typedef list<vector<int> *> matches_list;
typedef list<pair<int, int> > edge_list;
typedef list<pair<int, int> >::const_iterator edge_list_itr;
typedef int node_index_t;

DGTreeNode *DGTreeConstruct(map<int, Graph *> *data_graphs);
void treeGrow(DGTreeNode *root);
float score(DGTreeNode *node);
DG_Heap *candidateFeatures(DGTreeNode *node);
