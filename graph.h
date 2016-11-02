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
