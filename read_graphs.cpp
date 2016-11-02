#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <iterator>

using namespace std;

struct Graph {
  int id;
  int vertex_count;
  int edge_count;
  vector<string> vertex_labels; 
  vector<list <pair <int, int> >  > *adjacencyList;  
};

void printGraph(Graph &g) {
   cout << "==================================================" << endl;
   cout << "Graph id #"<< g.id << endl;
   cout << "Vertex count:"<< g.vertex_count << endl;
   cout << "Edge count:"<< g.edge_count << endl;
   cout << "--------------------------------------------------" << endl;

   vector<list<pair<int,int> > >::const_iterator itr = g.adjacencyList->begin();
   int vertex_index = 0;
   while (itr != g.adjacencyList->end()) {
        
       // print vertex label whose edge list is being printed 
       cout << "[" << g.vertex_labels[vertex_index] << "] -> "; 
       vertex_index++; 

       // print the edge list of the vertex 
       list<pair<int, int> >::const_iterator itl = (*itr).begin();
       while (itl != (*itr).end()) {
	   cout << g.vertex_labels[(*itl).first] << "(" << (*itl).second << ") " ;
	   itl++;
       } 
       cout << endl;
       itr++ ;
   }

   cout << "==================================================" << endl;

}

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

      data_graphs.push_back(g);

  } // end while reading all data graphs

  printGraph(*data_graphs[1]);
  return 0;
}
