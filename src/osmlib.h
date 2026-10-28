#include <sys/types.h>

struct road; // Forward declaration

typedef struct node {
  struct road *connects;
  uint amountOfConnects;
  double lattitude, longitude;
} node;

typedef struct road {
  int is_salted, prio;
  node *startNode, *endNode;
  double length;
} road;

struct road* setup(void);
double distance(struct node* node1, struct node* node2);
double findLength(struct road* road);
