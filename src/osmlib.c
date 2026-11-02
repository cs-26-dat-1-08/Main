#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

#include "osmlib.h"

// There are a maximum of 2000 nodes per road by the current standards. There can also be a larger amount of tags (also nodes in the data), but 1000 works so i think 2000 is future-proof
#define MAX_NODES_IN_ROAD 2000

void buildnode(node_t *awesome, xmlNode *node){
  xmlChar* tmp1 = xmlGetProp(node, (const xmlChar*)"id");
  xmlChar* tmp2 = xmlGetProp(node, (const xmlChar*)"lat");
  xmlChar* tmp3 = xmlGetProp(node, (const xmlChar*)"lon");
  awesome->id = atoi((char*)tmp1);
  awesome->lattitude = atof((char*)tmp2);
  awesome->longitude = atof((char*)tmp3);
  free(tmp1);
  free(tmp2);
  free(tmp3);
}

int validroad(protoroad_t *awesome, xmlNode *node){
  int count = 0;
  for (xmlNode *roadchild = node->children; roadchild; roadchild = roadchild->next){
    if(strcmp((char*)roadchild->name, "tag")) continue;
    xmlChar *tmp1 = xmlGetProp(roadchild, (const xmlChar*)"k");
    if(strcmp((char*)tmp1, "highway") == 0){
      char *tmp2 = (char*)xmlGetProp(roadchild, (const xmlChar*)"v");
      if(strcmp(tmp2, "motorway") == 0){
        // Håndteres ikke af kommunen
        awesome->prio = 4;
      } else if(strcmp(tmp2, "trunk") == 0){
        awesome->prio = 1;
      } else if(strcmp(tmp2, "primary") == 0){
        awesome->prio = 1;
      } else if(strcmp(tmp2, "secondary") == 0){
        awesome->prio = 1;
      } else if(strcmp(tmp2, "tertiary") == 0){
        awesome->prio = 2;
      } else if(strcmp(tmp2, "unclassified") == 0){
        awesome->prio = 4;
      } else if(strcmp(tmp2, "residential") == 0){
        awesome->prio = 4;
      } else if(strcmp(tmp2, "motorway_link") == 0){
        // Håndteres ikke af kommunen
        awesome->prio = 4;
      } else if(strcmp(tmp2, "trunk_link") == 0){
        awesome->prio = 1;
      } else if(strcmp(tmp2, "primary_link") == 0){
        awesome->prio = 1;
      } else if(strcmp(tmp2, "secondary_link") == 0){
        awesome->prio = 1;
      } else if(strcmp(tmp2, "tertiary_link") == 0){
        awesome->prio = 2;
      } else if(strcmp(tmp2, "living_street") == 0){
        awesome->prio = 4;
      } else if(strcmp(tmp2, "service") == 0){
        awesome->prio = 4;
      } else if(strcmp(tmp2, "track") == 0){
        awesome->prio = 4;
      } else if(strcmp(tmp2, "busway") == 0){
        awesome->prio = 1;
      } else {
        // IKke en vej, vi håndterer
      }
      free(tmp2);
    }
    free(tmp1);
    ++count;
    if(awesome->prio) return 1;
  }
  if(awesome->prio) return 1;
  return 0;
}

void buildroad(protoroad_t *awesome, xmlNode *node){
  xmlChar *tmp1 = xmlGetProp(node, (const xmlChar*)"id");
  awesome->id = atoi((char*)tmp1);
  free(tmp1);
  uint_fast32_t ids[MAX_NODES_IN_ROAD];
  uint_fast8_t count = 0;
  for (xmlNode *roadchild = node->children; roadchild; roadchild = roadchild->next){
    if(strcmp((char*)roadchild->name, "nd")) continue;
    xmlChar *tmp2 = xmlGetProp(roadchild, (const xmlChar*)"ref");
    ids[count] = atoi((char*)tmp2);
    free(tmp2);
    ++count;
  }
  uint_fast32_t newNodes[count];
  for(int i = 0; i < count; ++i)
    newNodes[i] = ids[i];
  awesome->nodeAmount = count;
  awesome->nodeIds = newNodes;
  return;
}

void setup(protoroad_t *roads[], node_t *nodes[], char *filename){
  uint_fast32_t nodeindex = 0, roadindex = 0;
  
  // xmlDoc *document = xmlReadFile(filename, NULL, 0);
  xmlDoc *document = xmlReadFile(filename, NULL, XML_PARSE_NOBLANKS);
  xmlNode *root = xmlDocGetRootElement(document);

  for (xmlNode *node = root->children; node; node = node->next) {
    switch (((char*) node->name)[0]){
      case 'n': // Node
        buildnode(nodes[nodeindex], node);
        ++nodeindex;
        break;
      case 'w': // Way
        if(!validroad(roads[roadindex], node)) continue;
        buildroad(roads[roadindex], node);
        ++roadindex;
        break;
      case 'r': // Relation
        break;
    }
  }
  fprintf(stdout, "Total nodes: %lu\n", nodeindex);
  fprintf(stdout, "Total roads: %lu\n", roadindex);

  xmlFreeDoc(document);
  xmlCleanupParser();
  return;
}

