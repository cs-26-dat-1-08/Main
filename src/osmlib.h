#include <stdint.h>
#include <sys/types.h>

struct road; // Forward declaration

typedef struct node {
  uint_fast32_t id;
  struct road *connects;
  uint_fast8_t amountOfConnects;
  double lattitude, longitude;
} node_t;

typedef struct protoroad {
  int id;
  uint_fast8_t prio;
  uint_fast8_t nodeAmount;
  uint_fast32_t *nodeIds;
} protoroad_t;

typedef struct road {
  uint_fast32_t id;
  uint_fast8_t is_salted, prio;
  node_t *startNode, *endNode;
  float length;
} road_t;

void setup(protoroad_t *roads[], node_t *nodes[], char *filename);
double distance(node_t* node1, node_t* node2);
double findLength(road_t* road);

void parse();
