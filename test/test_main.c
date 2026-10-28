#include "mtest.h"
#include "osmlib.h"

TEST_CASE(test_1,
    /* Arrange */

    /* Act */

    /* Assert */
    CHECK_EQ_INT(1,1);
)

/* Registrér hver test case i MAIN_RUN_TESTS */
MAIN_RUN_TESTS(test_1)
