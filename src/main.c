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

#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXNODES 2000000 /* In dataset: 1609300 */
#define MAXROADS 40000 /* In dataset: 32608 (With some filtering) */

int main(int arga, char** argb){
  /*
   * The program receives arga amounts of arguments, including where the file is.
   * As the program is currently, you call it with a path to a file.
   */
  switch (arga){
    default:
      fprintf(stderr, "Usage: %s FILE.osm\n", argb[0]);
      return 1;
    case 2:
      // TODO: Input sanitasion, make sure argb[1] is a proper filepath.
      break;
  }

  /*
   * Declarations of the arrays of pointers to structs.
   */
  static node_t *nodes[MAXNODES];
  static road_t *roads[MAXROADS];
  for(int i = 0; i < MAXNODES; ++i){
    nodes[i] = calloc(sizeof(node_t), 1);
    nodes[i]->connects = calloc(sizeof(road_t*), MAX_ROADS_IN_NODE);
  }
  for(int i = 0; i < MAXROADS; ++i){
    roads[i] = calloc(sizeof(road_t), 1);
  }

  /* Setting up the structs, check osmlib.c/h */  
  setup(roads, nodes, argb[1]);

  /* Freeing up the mallocs */
  for(int i = 0; i < MAXNODES; ++i){
    free(nodes[i]->connects);
    free(nodes[i]);
  }
  for(int i = 0; i < MAXROADS; ++i){
    free(roads[i]);
  }

  return 0;
}
