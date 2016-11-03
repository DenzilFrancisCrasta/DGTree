#include <iostream>
#include "dgtree.h"

int main() {
  char c; 
  //vector<Graph *> data_graphs;
  map<int, Graph *> data_graphs; 

  int total_vertices = 0;

  while (cin >> c) {
      Graph *g = new Graph; 

      cin >> g->id;
      cin >> g->vertex_count;
      total_vertices += g->vertex_count;

      // Read in the vertex labels 
      string vlabel;
      for (int i=0; i< (g->vertex_count); i++) {
          cin >> vlabel; 
          g->vertex_labels.push_back(vlabel);
      }


      g->adjacencyList = new vector<list<pair<int, int> > >(g->vertex_count);
      
      int v1, v2, valence;
      cin >> g->edge_count;
      for (int i=0; i< (g->edge_count); i++) {
          cin >> v1 >> v2 >> valence;
          (*g->adjacencyList)[v1].push_back(make_pair(v2, valence)); 
          (*g->adjacencyList)[v2].push_back(make_pair(v1, valence)); 
      }

      data_graphs[g->id] = g;

  } // end while reading all data graphs

  //printGraph(*data_graphs[1]);
 
  cout << "total vertices " << total_vertices <<endl;

  //We have read all the data-graphs We can now start building the DGTree
  DGTreeNode *root = DGTreeConstruct(&data_graphs);

  cout << root->matches_of->size() << endl;
  return 0;
}
