#include "unity.h"
#include "problem_solver.h"

void setUp(void) {
}

void tearDown(void) {
}

/** Calculate how many digits in a (positive) number
 */
int ndigs( ull n )
{
    if( n == 0 ){
        return 1;
    }

    int digs = 0;
    while( n > 0 ){
        digs ++;
        n /= 10;
    }

    return digs;
}

void test_1() {
    TEST_ASSERT_EQUAL( 1, ndigs(0) );
    TEST_ASSERT_EQUAL( 1, ndigs(1) );
    TEST_ASSERT_EQUAL( 1, ndigs(9) );

    TEST_ASSERT_EQUAL( 2, ndigs(10) );
    TEST_ASSERT_EQUAL( 2, ndigs(19) );
    TEST_ASSERT_EQUAL( 2, ndigs(20) );
    TEST_ASSERT_EQUAL( 2, ndigs(99) );

    TEST_ASSERT_EQUAL( 3, ndigs(100) );
    TEST_ASSERT_EQUAL( 3, ndigs(999) );

    TEST_ASSERT_EQUAL( 4, ndigs(1000) );
}

void test_many()
{
    TEST_ASSERT_EQUAL( 1, ndigs(1) );
    TEST_ASSERT_EQUAL( 1, ndigs(9) );

    TEST_ASSERT_EQUAL( 2, ndigs(11) );
    TEST_ASSERT_EQUAL( 2, ndigs(99) );

    TEST_ASSERT_EQUAL( 3, ndigs(111) );
    TEST_ASSERT_EQUAL( 3, ndigs(999) );

    TEST_ASSERT_EQUAL( 4, ndigs(1111) );
    TEST_ASSERT_EQUAL( 4, ndigs(9999) );

    TEST_ASSERT_EQUAL( 5, ndigs(11111) );
    TEST_ASSERT_EQUAL( 5, ndigs(99999) );

    TEST_ASSERT_EQUAL( 6, ndigs(111111) );
    TEST_ASSERT_EQUAL( 6, ndigs(999999) );

    TEST_ASSERT_EQUAL( 7, ndigs(1111111) );
    TEST_ASSERT_EQUAL( 7, ndigs(9999999) );

    TEST_ASSERT_EQUAL( 8, ndigs(11111111) );
    TEST_ASSERT_EQUAL( 8, ndigs(99999999) );

    TEST_ASSERT_EQUAL( 9, ndigs(111111111) );
    TEST_ASSERT_EQUAL( 9, ndigs(999999999) );

    TEST_ASSERT_EQUAL( 10, ndigs(1111111111) );
    TEST_ASSERT_EQUAL( 10, ndigs(9999999999) );

    TEST_ASSERT_EQUAL( 11, ndigs(11111111111) );
    TEST_ASSERT_EQUAL( 11, ndigs(99999999999) );

    TEST_ASSERT_EQUAL( 12, ndigs(111111111111) );
    TEST_ASSERT_EQUAL( 12, ndigs(999999999999) );

    TEST_ASSERT_EQUAL( 13, ndigs(1111111111111) );
    TEST_ASSERT_EQUAL( 13, ndigs(9999999999999) );

    TEST_ASSERT_EQUAL( 14, ndigs(11111111111111) );
    TEST_ASSERT_EQUAL( 14, ndigs(99999999999999) );

    TEST_ASSERT_EQUAL( 15, ndigs(111111111111111) );
    TEST_ASSERT_EQUAL( 15, ndigs(999999999999999) );

    TEST_ASSERT_EQUAL( 16, ndigs(1111111111111111) );
    TEST_ASSERT_EQUAL( 16, ndigs(9999999999999999) );

    TEST_ASSERT_EQUAL( 17, ndigs(11111111111111111) );
    TEST_ASSERT_EQUAL( 17, ndigs(99999999999999999) );

    TEST_ASSERT_EQUAL( 18, ndigs(111111111111111111) );
    TEST_ASSERT_EQUAL( 18, ndigs(999999999999999999) );
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_1);
    RUN_TEST(test_many);

    return UNITY_END();
}
