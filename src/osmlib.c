#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

#include "osmlib.h"

#define MAX_NODES_IN_ROAD 100

void buildroad(protoroad *awesome, uint size, int *nodes){
  int newNodes[size];
  for(int i = 0; i < size; ++i)
    newNodes[i] = nodes[i];
  awesome->nodeAmount = size;
  awesome->nodeIds = newNodes;
}
void setup(protoroad **roads, node **nodes, char *filename){
  int nodeindex = 0, roadindex = 0;

  xmlDoc *document = xmlReadFile(filename, NULL, XML_PARSE_NOBLANKS);
  xmlNode *root = xmlDocGetRootElement(document);

  for (xmlNode *node = root->children; node; node = node->next) {
    switch (strcmp((char*) node-> name, "way")){
      case -9: // Node
        nodes[nodeindex]->id = atoi((char*)xmlGetProp(node, (const xmlChar*)"id"));
        nodes[nodeindex]->lattitude = atof((char*)xmlGetProp(node, (const xmlChar*)"lat"));
        nodes[nodeindex]->longitude = atof((char*)xmlGetProp(node, (const xmlChar*)"lon"));
        ++nodeindex;
        break;
      case 0: // Way
        roads[roadindex]->id = atoi((char*)xmlGetProp(node, (const xmlChar*)"id"));
        int ids[MAX_NODES_IN_ROAD];
        uint count = 0;
        for (xmlNode *roadchild = node->children; roadchild; roadchild = roadchild->next){
          if(strcmp((char*)roadchild->name, "nd")) continue;
          ids[count] = atoi((char*)xmlGetProp(roadchild, (const xmlChar*)"ref"));
          ++count;
        }
        buildroad(roads[roadindex], count, ids);
        ++roadindex;
        break;
      case -5: // Relation
        break;
    }
  }
  printf("Total nodes: %i\n",nodeindex);
  printf("Total roads: %i\n",roadindex);
  fprintf(stdout, "...\n");
  return;
}

