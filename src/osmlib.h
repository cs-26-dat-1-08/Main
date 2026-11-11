#include <stdint.h>
#include <sys/types.h>

// Max var 19 i aalborg datasæt.
#define MAX_ROADS_IN_NODE 32

struct protoroad; // Forward declaration

typedef uint_fast8_t u8;
typedef uint_fast64_t u64;
typedef int_fast64_t i64;
typedef i64 nid_t;

typedef struct node {
  nid_t id;
  struct protoroad **connects;
  u64 amountOfConnects;
  double latitude, longitude;
} node_t;

typedef struct protoroad {
  nid_t id;
  u8 prio;
  u64 nodeAmount;
  nid_t *nodeIds;
} protoroad_t;

typedef struct road {
  nid_t id;
  u8 is_salted, prio;
  node_t *startNode, *endNode;
  double length;
} road_t;

void setup(protoroad_t *roads[], node_t *nodes[], char *filename);
double node_distance(node_t node1, node_t node2);
double findLength(node_t *nodes[], u64 size, protoroad_t* road);
node_t **node_from_id(node_t *nodes[], u64 size, nid_t id);
