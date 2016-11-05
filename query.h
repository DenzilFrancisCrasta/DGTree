#include <vector>
#include "dgtree.h"
#include <algorithm>

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
vector<int> *search(DGTreeNode *root, Graph *Q);
