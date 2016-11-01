#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

using namespace std;

struct Graph {
  int id;
  int vertex_count;
  int edge_count;
  vector<string> vertex_labels; 
  vector<list <pair <int, int> >  > *adjacencyList;  
};

int main() {
  char c; 
  int vertex_count, edge_count;
  vector<Graph *> data_graphs;

  while (cin >> c) {
      Graph *g = new Graph; 

      cin >> g->id;
      cin >> g->vertex_count;

      // Read in the vertex labels 
      string vlabel;
      for (int i=0; i<vertex_count; i++) {
          cin >> vlabel; 
          g->vertex_labels.push_back(vlabel);
      }


      g->adjacencyList = new vector<list<pair<int, int> > >(vertex_count);
      int v1, v2, valence;
      cin >> g->edge_count;
      for (int i=0; i<edge_count; i++) {
          cin >> v1 >> v2 >> valence;
          (*g->adjacencyList)[v1].push(make_pair(v2, valence)); 
          (*g->adjacencyList)[v2].push(make_pair(v1, valence)); 
      }

      data_graphs.push_back(g);

  } // end while reading all data graphs

  cout << data_graphs.size() << endl;

   
  return 0;
}
