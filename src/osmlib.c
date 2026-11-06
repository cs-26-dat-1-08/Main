/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2026 Asger Peter Fuglsbjerg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

#include "osmlib.h"

#define EARTH_RADIUS 6371009
// There are a maximum of 2000 nodes per road by the current standards. There can also be a larger amount of tags (also nodes in the data), but 1000 works so i think 2000 is future-proof
#define MAX_NODES_IN_ROAD 2000

void buildnode(node_t *awesome, xmlNode *node){
  xmlChar* tmp1 = xmlGetProp(node, (const xmlChar*)"id");
  xmlChar* tmp2 = xmlGetProp(node, (const xmlChar*)"lat");
  xmlChar* tmp3 = xmlGetProp(node, (const xmlChar*)"lon");
  awesome->id = (nid_t)atoll((char*)tmp1);
  awesome->latitude = atof((char*)tmp2);
  awesome->longitude = atof((char*)tmp3);
  free(tmp1);
  free(tmp2);
  free(tmp3);
}

int validroad(protoroad_t *awesome, xmlNode *node){
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
    if(awesome->prio) return 1;
  }
  return 0;
}

int compareNodes(const void *a, const void *b){
  nid_t tmp1 = (*(const node_t**)a)->id;
  nid_t tmp2 = (*(const node_t**)b)->id;
  return tmp1 - tmp2;
}

void buildroad(protoroad_t *awesome, xmlNode *node){
  xmlChar *tmp1 = xmlGetProp(node, (const xmlChar*)"id");
  awesome->id = (nid_t)atoll((char*)tmp1);
  free(tmp1);
  nid_t ids[MAX_NODES_IN_ROAD];
  u8 count = 0;
  for (xmlNode *roadchild = node->children; roadchild; roadchild = roadchild->next){
    if(strcmp((char*)roadchild->name, "nd")) continue;
    xmlChar *tmp2 = xmlGetProp(roadchild, (const xmlChar*)"ref");
    ids[count] = (nid_t)atoll((char*)tmp2);
    free(tmp2);
    ++count;
  }
  nid_t newNodes[count];
  for(uint i = 0; i < count; ++i)
    newNodes[i] = ids[i];
  awesome->nodeAmount = count;
  awesome->nodeIds = newNodes;
  return;
}

void setup(protoroad_t *roads[], node_t *nodes[], char *filename){
  u64 nodeindex = 0, roadindex = 0;
  
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
        if(!validroad(roads[roadindex], node)) break;
        buildroad(roads[roadindex], node);
        ++roadindex;
        break;
      case 'r': // Relation
        break;
    }
  }

  // Datasættet burde være sorteret, men for at være sikker, så kører vi lige qsort.
  qsort(nodes, nodeindex, sizeof(nodes[0]), compareNodes);  
  
  for (uint i = 0; i < roadindex; ++i){
    // findLength(roads[i]);
    for (uint j = 0; j < roads[i]->nodeAmount; ++j){
      node_t balls = {roads[i]->nodeIds[j]};
      node_t *bp = &balls;
      node_t **foundNode = (node_t**)bsearch(&bp, nodes, nodeindex, sizeof(nodes[0]), compareNodes);
      if(foundNode == NULL) continue;
      (*foundNode)->connects[(*foundNode)->amountOfConnects] = roads[i];
      ++(*foundNode)->amountOfConnects;
    }
  }

  /* TODO: Fix below code and make findLength real */
  
  // uint count = 0;
  // for (uint i = 0; i < nodeindex; ++i)
    // if(nodes[i]->amountOfConnects > 1) ++count;
  // node_t* newNodes[count];
  // for (uint i = 0, j = 0; i < nodeindex; ++i){
    // if(nodes[i]->amountOfConnects <= 1) continue;
    // newNodes[j] = nodes[i];
    // ++j;
  // }
  // for (uint i = 0; i < count; ++i){
    // u8 aoc = newNodes[i]->amountOfConnects;
    // for(u8 j = 0; j < aoc; ++j){
      // u8 k = 0;
      // for(;newNodes[i]->connects[j]->nodes[k+1] == NULL; ++k){}
      // newNodes[i]->connects[j]->nodes[k] = newNodes[i];
    // }
  // }
  
  fprintf(stdout, "Total nodes: %lu\n", nodeindex);
  fprintf(stdout, "Total roads: %lu\n", roadindex);

  xmlFreeDoc(document);
  xmlCleanupParser();
  return;
}

double node_distance(node_t* node1, node_t* node2){
  // Antager at der kun bliver udregnet meget korte afstande.
  // https://en.wikipedia.org/wiki/Geographical_distance#Tunnel_distance

  double deltaX = cos(node2->latitude)*cos(node2->longitude)-cos(node1->latitude)*cos(node1->longitude);
  double deltaY = cos(node2->latitude)*sin(node2->longitude)-cos(node1->latitude)*sin(node1->longitude);
  double deltaZ = sin(node2->latitude)-sin(node1->latitude);
  double D = EARTH_RADIUS*sqrt(pow(deltaX,2)+pow(deltaY,2)+pow(deltaZ,2));
  return D;
}
// double findLength(protoroad_t* road){
  // double distance = 0;
  // for(uint i = 1; i < road->nodeAmount; ++i)
    // distance += node_distance(road->nodes[i-1], road->nodes[i]);
  // return distance;
// }
