#include <stdint.h>
#include <sys/types.h>

struct road; // Forward declaration

typedef struct node {
  uint_fast32_t id;
  struct road *connects;
  uint_fast8_t amountOfConnects;
  double lattitude, longitude;
} node;

typedef struct protoroad {
  int id;
  uint_fast8_t prio;
  uint_fast8_t nodeAmount;
  uint_fast32_t *nodeIds;
} protoroad;

typedef struct road {
  uint_fast32_t id;
  uint_fast8_t is_salted, prio;
  node *startNode, *endNode;
  float length;
} road;

void setup(protoroad *roads[], node *nodes[], char *filename);
double distance(struct node* node1, struct node* node2);
double findLength(struct road* road);

void parse();
