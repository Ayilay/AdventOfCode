#include "unity.h"
#include "common.h"
#include "problem_solver.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_1() {
    TEST_ASSERT_LESS_OR_EQUAL( 8, sizeof(struct factoryState) );
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_1);
    return UNITY_END();
}
