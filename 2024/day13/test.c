#include "unity.h"
#include "problem_solver.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_1() {
    TEST_ASSERT_EQUAL(  1, ctz( 1 <<  1) );
    TEST_ASSERT_EQUAL(  2, ctz( 1 <<  2) );
    TEST_ASSERT_EQUAL(  3, ctz( 1 <<  3) );
    TEST_ASSERT_EQUAL(  4, ctz( 1 <<  4) );
    TEST_ASSERT_EQUAL( 15, ctz( 1 << 15) );
}

void test_2() {
    TEST_ASSERT_EQUAL( 2, gcd(94, 34) );
    TEST_ASSERT_EQUAL( 2775, gcd(2775, 111000) );
    TEST_ASSERT_EQUAL( 94, gcd(94, 7520) );
}

void test_zero() {
    TEST_ASSERT_EQUAL( 0, gcd(0, 0) );
    TEST_ASSERT_EQUAL( 1, gcd(1, 0) );
    TEST_ASSERT_EQUAL( 1, gcd(0, 1) );
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_1);
    RUN_TEST(test_2);
    RUN_TEST(test_zero);
    return UNITY_END();
}
