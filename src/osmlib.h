#include <sys/types.h>

struct road; // Forward declaration

typedef struct node {
  int id;
  struct road *connects;
  uint amountOfConnects;
  double lattitude, longitude;
} node;

typedef struct protoroad {
  int id;
  int prio;
  int nodeAmount;
  int *nodeIds;
} protoroad;

typedef struct road {
  int id;
  int is_salted, prio;
  node *startNode, *endNode;
  double length;
} road;

void setup(protoroad **roads, node **nodes, char *filename);
double distance(struct node* node1, struct node* node2);
double findLength(struct road* road);

void parse();
