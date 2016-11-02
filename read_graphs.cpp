#include <iostream>
#include <string>
#include "graph.h"

using namespace std;

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

