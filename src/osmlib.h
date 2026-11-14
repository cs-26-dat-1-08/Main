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
#include <sys/types.h>

// Max var 19 i aalborg datasæt.
#define MAX_ROADS_IN_NODE 32

struct protoroad; // Forward declaration

/*
 * typedefs that make it much easier to control the int types.
 */
typedef uint_fast8_t u8;
typedef uint_fast64_t u64;
typedef int_fast64_t i64;
typedef i64 nid_t;

/*
 * Struct that has all of the necesaary information about a node.
 */
typedef struct node {
  nid_t id;
  struct protoroad **connects;
  u64 amountOfConnects;
  double latitude, longitude;
} node_t;

/*
 * Struct that has all of the necesaary information about a road.
 */
typedef struct road {
  nid_t id;
  u8 is_salted, prio;
  u64 nodeAmount;
  nid_t *nodeIds;
  double length;
  // node_t *startNode, *endNode; // Depreciated, use nodeIds[0] and nodeIds[nodeAmount-1] instead.
} road_t;

/*
 * Sets up the struct pointer arrays roads and nodes
 *
 * @param[out] *road_t[] road_t-array of pre-malloc'ed pointers to roads.
 * @param[out] *node_t[] node_t-array of pre-malloc'ed pointers to nodes.
 * @param[in] *char char-array (String) to a .osm file that is read from.
 */
void setup(road_t *roads[], node_t *nodes[], char *filename);
/*
 * Finds the distance between two nodes
 *
 * @param[in] node1 a node
 * @param[in] node2 a node
 * @return double distance between the inputted nodes
 */
double node_distance(node_t node1, node_t node2);

/*
 * Finds the total distance between all nodes in a given road.
 * Can be regarded as the length of the road.
 * Note: road is not written to.
 *
 * @param[in] *node_t[] node_t-array of pre-malloc'ed pointers to the domain of nodes.
 * @param[in] u64 size of the node_t-array.
 * @param[in] road_t* pointer to the road being evaluated.
 * @return double length of the road.
 */
double findLength(node_t *nodes[], u64 size, road_t* road);

/*
 * Returns a double-pointer to a node from a id.
 * Returns a NULL pointer if the node cannot be found.
 *
 * @param[in] *node_t[] node_t-array of pre-malloc'ed pointers to the domain of nodes.
 * @param[in] u64 size of the node_t-array.
 * @param[in] nid_t id of the node to be found.
 * @return node_t** pointer to pointer of the found node.
 */
node_t **node_from_id(node_t *nodes[], u64 size, nid_t id);
