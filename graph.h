#include <vector>
#include <list>
#include <utility>
#include <iterator>

using namespace std;

typedef vector<list<pair<int, int> > > adj_list_t;

struct Graph {
  int id;
  int vertex_count;
  int edge_count;
  vector<string> vertex_labels; 
  adj_list_t *adjacencyList;  
};

void printGraph(Graph &g);
