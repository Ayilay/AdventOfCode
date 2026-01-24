#include "unity.h"
#include "problem_solver.h"
#include "tree.h"

void setUp(void) {
}

void tearDown(void) {
}

int tree_compar_int( void* a, void* b )
{

}

void test_1() {
    Tree tree = TREE_init( tree_compar_int );

    TEST_ASSERT_TRUE( 1 );
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_1);
    return UNITY_END();
}
