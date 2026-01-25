#include "unity.h"
#include "problem_solver.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_1() {
    TEST_FAIL_MESSAGE( "test not found");
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_1);
    return UNITY_END();
}
