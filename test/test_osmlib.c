#include "mtest.h"
#include "osmlib.h"

TEST_CASE(test_node_from_id,
    /* Arrange */
    node_t node1 = {.id = 67};
    node_t node2 = {.id = 68};
    node_t node3 = {.id = 69};
    node_t node4 = {.id = 70};
    node_t node5 = {.id = 71};
    node_t node6 = {.id = 12631};
    node_t *nodes[6];
    nodes[0] = &node1;
    nodes[1] = &node2;
    nodes[2] = &node3;
    nodes[3] = &node4;
    nodes[4] = &node5;
    nodes[5] = &node6;

    /* Act */
    node_t **test1 = node_from_id(nodes, 6, 67);
    node_t **test2 = node_from_id(nodes, 6, 68);
    node_t **test3 = node_from_id(nodes, 6, 69);
    node_t **test4 = node_from_id(nodes, 6, 70);
    node_t **test5 = node_from_id(nodes, 6, 71);
    node_t **test6 = node_from_id(nodes, 6, 12631);
    node_t **testnull = node_from_id(nodes, 6, 5);

    /* Assert */
    CHECK((*test1)->id == 67, "node_from_id regular failed");
    CHECK((*test2)->id == 68, "node_from_id regular failed");
    CHECK((*test3)->id == 69, "node_from_id regular failed");
    CHECK((*test4)->id == 70, "node_from_id regular failed");
    CHECK((*test5)->id == 71, "node_from_id regular failed");
    CHECK((*test6)->id == 12631, "node_from_id regular failed");
    CHECK(testnull == NULL, "node_from_id null failed");
)

TEST_CASE(test_findlength,
    node_t node1 = {.latitude = 57.0308277, .longitude = 9.9555626};
    node_t node2 = {.latitude = 57.0452820, .longitude = 9.9244230};
    node_t node3 = {.latitude = 57.0480507, .longitude = 9.9219458};
    node_t node4 = {.latitude = 57.0480895, .longitude = 9.9207442};

    double testzero = node_distance(node1, node1);
    double testsome = node_distance(node3, node4);
    double testlong = node_distance(node1, node2);

    CHECK_EQ_DOUBLE(0.0, testzero, 0.01);
    CHECK_EQ_DOUBLE(73.0, testsome, 0.1);
    CHECK_EQ_DOUBLE(2482.9, testlong, 1.0);
)

/* Registrér hver test case i MAIN_RUN_TESTS */
MAIN_RUN_TESTS(test_node_from_id, test_findlength)
