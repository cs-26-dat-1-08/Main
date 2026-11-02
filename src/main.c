#include "algorithms.h"

#define FILENAME "/home/asger/osm-import/aalborg.osm"
//#define FILENAME "/home/asger/osm-import/test.osm"

#define MAXNODES 1609301 /* 1609300 + 1 */
#define MAXROADS 212230 /* 212229 + 1 */

node* nodes[MAXNODES];
protoroad* roads[MAXROADS];

int main(void){
  setup(roads, nodes, FILENAME);
  return 0;
}
