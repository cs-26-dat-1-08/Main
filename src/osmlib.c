/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2026
      Anas Ahmed Yusuf
      Asger Peter Fuglsbjerg
      Jonatan Bjørn Rasmussen
      Magnus Hjortshøj Jørgensen
      Nikolaj Frederik Faurboe Horn

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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include "geodesic.h"

#include "osmlib.h"

// There are a maximum of 2000 nodes per road by the current standards. There can also be a larger amount of tags (also nodes in the data), but 1000 works so i think 2000 is VERY future-proof
#define MAX_NODES_IN_ROAD 2000

void buildnode(node_t *awesome, xmlNode *node){
  xmlChar* tmp1 = xmlGetProp(node, (const xmlChar*)"id");
  xmlChar* tmp2 = xmlGetProp(node, (const xmlChar*)"lat");
  xmlChar* tmp3 = xmlGetProp(node, (const xmlChar*)"lon");
  awesome->id = (nid_t)atol((char*)tmp1);
  awesome->latitude = atof((char*)tmp2);
  awesome->longitude = atof((char*)tmp3);
  free(tmp1);
  free(tmp2);
  free(tmp3);
}

int validroad(road_t *awesome, xmlNode *node){
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
  if(tmp1 > tmp2) return 1;
  if(tmp1 < tmp2) return -1;
  return 0;
}

void buildroad(road_t *awesome, xmlNode *node){
  xmlChar *tmp1 = xmlGetProp(node, (const xmlChar*)"id");
  awesome->id = (nid_t)atol((char*)tmp1);
  free(tmp1);
  nid_t ids[MAX_NODES_IN_ROAD];
  u64 count = 0;
  for (xmlNode *roadchild = node->children; roadchild; roadchild = roadchild->next){
    if(strcmp((char*)roadchild->name, "nd") != 0) continue;
    xmlChar *tmp2 = xmlGetProp(roadchild, (const xmlChar*)"ref");
    ids[count] = (nid_t)atol((char*)tmp2);
    free(tmp2);
    ++count;
  }
  nid_t newNodes[count];
  for(u64 i = 0; i < count; ++i)
    newNodes[i] = ids[i];
  awesome->nodeAmount = count;
  awesome->nodeIds = newNodes;
  return;
}

void setup(road_t *roads[], node_t *nodes[], char *filename){
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

  // Datasættet burde være sorteret, da det er sorteret efter ID når man downloader det, men for at være sikker, så kører vi lige qsort.
  qsort(nodes, nodeindex, sizeof(nodes[0]), compareNodes);  
  
  fprintf(stdout, "Total nodes: %lu\n", nodeindex);
  fprintf(stdout, "Total roads: %lu\n", roadindex);

  // TODO: Fix denne SEGV
  // for (u64 i = 0; i < roadindex; ++i){
    // findLength(nodes, nodeindex, roads[i]);
  // }

  // TODO: Lav amountOfConnects og Connects på alle nodes. Malfungerende eksempel er nedenfor
// void awesome(protoroad_t *road, node_t *nodes[], u64 nodeindex){
//   node_t **fn, *foundNode;
//   // findLength(nodes, nodeindex, roads[i]);
//   for (u64 j = 0; j < road->nodeAmount; ++j){
//     fn = node_from_id(nodes, nodeindex, road->nodeIds[j]);
//     if(fn == NULL) continue;
//     foundNode = *fn;
//     if(fn == NULL) continue;
//     foundNode->connects[foundNode->amountOfConnects] = road;
//     ++foundNode->amountOfConnects;
//     fn = NULL;
//   }
// }

  // TODO: Filtrer yderligere nodes ud. Malfungerende eksempel er nedenfor.
  // uint count = 0;
  // for (uint i = 0; i < nodeindex; ++i)
  //   if(nodes[i]->amountOfConnects > 1) ++count;
  // node_t* newNodes[count];
  // for (uint i = 0, j = 0; i < nodeindex; ++i){
  //   if(nodes[i]->amountOfConnects > 1){
  //     newNodes[j] = nodes[i];
  //     ++j;
  //   }
  // }
  
  // fprintf(stdout, "Total nodes: %u (Filtered)\n", count);

  xmlFreeDoc(document);
  xmlCleanupParser();
  return;
}

double node_distance(node_t node1, node_t node2){
  //using https://geographiclib.sourceforge.io/C/doc/standalone.html
  double a = 6378137.0, f = 1/298.257223563, D;
  double lat1 = node1.latitude, lon1 = node1.longitude, lat2 = node2.latitude, lon2 = node2.longitude;
  struct geod_geodesic g;
  geod_init(&g, a, f);
  geod_inverse(&g, lat1, lon1, lat2, lon2, &D, 0, 0);
  return D;
}

// TODO: Doesn't seem to work properly.
node_t **node_from_id(node_t *nodes[], u64 size, nid_t id){
  // node_t **foundNode = NULL;
  // for(uint i = 0; i < size; ++i){
  //   if(nodes[i]->id == id) foundNode = &nodes[i];
  // }
  
  node_t balls = {.id = id};
  node_t *bp = &balls;
  node_t **foundNode = bsearch(&bp, nodes, size, sizeof(nodes[0]), compareNodes);
  if (foundNode == NULL) fprintf(stderr, "Could not find node with ID: %lu\n", id); // Temporary debug information
  return foundNode;
}

double findLength(node_t *nodes[], u64 size, road_t* road){
  double distance = 0;
  for(uint i = 1; i < road->nodeAmount; ++i){
    node_t **tmp1 = node_from_id(nodes, size, road->nodeIds[i-1]);
    node_t **tmp2 = node_from_id(nodes, size, road->nodeIds[i]);
    if(tmp1 != NULL && tmp2 != NULL)
      distance += node_distance(**tmp1, **tmp2);
    // else {
    //   printf("Could not find nodes in road %lu\n", road->id);
    //   printf("  1: %lu & %p\n", road->nodeIds[i-1], tmp1);
    //   printf("  2: %lu & %p\n", road->nodeIds[i], tmp2);
    // }
  }
  return distance;
}
