#include <iostream>
#include <fstream>

using namespace std;
int main() {

   const char *filename = "asmall.txt";
   ifstream infile(filename);

   if (!infile) {
       cerr << "ERROR: Failed to open file " << filename << endl;
       return 1;
   }

   int n, x, y, v;
   char c;
   string label;

   while (infile >> c) {

      // infile >> c;
       infile >> n;
       cout << "#" << n << endl;
       infile >> n; 
       cout << n << endl;

       for (int i=0; i<n; i++) {
           infile >> label;
           cout << label << endl;
       }

       infile >> n; 
       cout << n << endl;
       for (int i=0; i<n; i++) {
           infile >> x >> y >> v;
           cout << x  <<" " << y <<" " << v << endl;
       }
   }


} 
