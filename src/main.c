#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXNODES 1609301 /* 1609300 + 1 */
#define MAXROADS 212230 /* 212229 + 1 */

// #define MAXNODES 1000 /* 1609300 + 1 */
// #define MAXROADS 100 /* 212229 + 1 */

int main(int arga, char** argb){
  // node *nodes = malloc(MAXNODES*sizeof(node));
  // protoroad *roads = malloc(MAXNODES*sizeof(protoroad));

  static node *nodes[MAXNODES];
  static protoroad *roads[MAXROADS];
  for(int i = 0; i < MAXNODES; ++i)
    nodes[i] = malloc(sizeof(node));
  for(int i = 0; i < MAXROADS; ++i)
    roads[i] = malloc(sizeof(protoroad));
  
  
  switch (arga){
    case 1:
    default:
      fprintf(stderr, "Usage: %s FILE.osm\n", argb[0]);
      return 1;
    case 2:
      break;
  }
  setup(roads, nodes, argb[1]);

  for(int i = 0; i < MAXNODES; ++i)
    free(nodes[i]);
  for(int i = 0; i < MAXROADS; ++i)
    free(roads[i]);

  return 0;
}
