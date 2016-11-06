#include <vector>
#include <algorithm>
#include "dgtree.h"

struct Entry {
  DGTreeNode *treenode; 
  map<int, Graph *> *S_star;
  list<vector<int> *> *matches;
  float score;
}; 

struct compare_entries {
  bool operator()(Entry *node_a, Entry *node_b) {
      return node_a->score < node_b->score;
  }
};

typedef priority_queue<Entry *, vector<Entry *>, compare_entries> Q_Heap; 
Entry * BestFeature(Q_Heap *H, map<int, Graph *> *C );
map<int, Graph *> *search(DGTreeNode *root, Graph *Q);
void FeatureExpansion(Graph *Q, Entry *q, DGTreeNode *g_plus, Q_Heap *H, map<int, Graph *> *C);
