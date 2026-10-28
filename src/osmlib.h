#include <sys/types.h>

typedef struct NODE node;
typedef struct ROAD road;

struct NODE {
  road* connects;
  uint amountOfConnects;
  double lattitude, longitude;
};

struct ROAD {
  int is_salted, prio;
  node *startNode, endNode;
  double length;
};

road* setup(void);
double distance(node* node1, node* node2);
double findLength(road* road);
