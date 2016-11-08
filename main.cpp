#include <iostream>
#include <fstream>
#include "query.h"

int main(int argc, char **argv) {
  char c; 
  char *data_file = argv[1];
  char *query_file = argv[2];
  ifstream infile(data_file);
 
  if (!infile) {
     cerr << "ERROR: Failed to open file " << data_file << endl;
     return 1;
  }

  map<int, Graph *> data_graphs; 

  int total_vertices = 0;

  while (infile >> c) {
      Graph *g = new Graph; 

      infile >> g->id;
      infile >> g->vertex_count;
      total_vertices += g->vertex_count;

      // Read in the vertex labels 
      string vlabel;
      for (int i=0; i< (g->vertex_count); i++) {
          infile >> vlabel; 
          g->vertex_labels.push_back(vlabel);
      }


      g->adjacencyList = new vector<list<pair<int, int> > >(g->vertex_count);
      
      int v1, v2, valence;
      infile >> g->edge_count;
      for (int i=0; i< (g->edge_count); i++) {
          infile >> v1 >> v2 >> valence;
          (*g->adjacencyList)[v1].push_back(make_pair(v2, valence)); 
          (*g->adjacencyList)[v2].push_back(make_pair(v1, valence)); 
      }

      data_graphs[g->id] = g;

  } // end while reading all data graphs

  //printGraph(*data_graphs[1]);
 
  //cout << "total vertices " << total_vertices <<endl;

  //We have read all the data-graphs We can now start building the DGTree
  DGTreeNode *root = DGTreeConstruct(&data_graphs);
  //cout << root->matches_of->size() << endl;

  ifstream qfile(query_file);
  if (!qfile) {
     cerr << "ERROR: Failed to open file " << query_file << endl;
     return 1;
  }

  while (qfile >> c) {

      Graph *g = new Graph; 

      qfile >> g->id;
      qfile >> g->vertex_count;
      //total_vertices += g->vertex_count;

      // Read in the vertex labels 
      string vlabel;
      for (int i=0; i< (g->vertex_count); i++) {
          qfile >> vlabel; 
          g->vertex_labels.push_back(vlabel);
      }


      g->adjacencyList = new vector<list<pair<int, int> > >(g->vertex_count);
      
      int v1, v2, valence;
      qfile >> g->edge_count;
      for (int i=0; i< (g->edge_count); i++) {
          qfile >> v1 >> v2 >> valence;
          (*g->adjacencyList)[v1].push_back(make_pair(v2, valence)); 
          (*g->adjacencyList)[v2].push_back(make_pair(v1, valence)); 
      }

      map<int, Graph *> *m = search(root, g); 

      cout << m->size() << endl;

      for (map<int, Graph *>::const_iterator itr=m->begin(); itr != m->end(); itr++)
          cout << itr->first << " " ;
      cout << endl;
      //data_graphs[g->id] = g;
      

  } // end while reading all data graphs
  

  return 0;
}
