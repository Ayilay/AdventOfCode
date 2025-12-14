#include "unity.h"
#include "math.h"
#include "problem_solver.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_additional() {
    TEST_ASSERT_EQUAL( 0, isIllegal(101) );
}

void test_1() {
    TEST_ASSERT_EQUAL( 0, isIllegal(1) );

    // Test cases from problem description
    TEST_ASSERT_EQUAL( 1, isIllegal(11) );
    TEST_ASSERT_EQUAL( 1, isIllegal(22) );

    TEST_ASSERT_EQUAL( 1, isIllegal(99) );
    TEST_ASSERT_EQUAL( 1, isIllegal(1010) );

    TEST_ASSERT_EQUAL( 1, isIllegal(1188511885) );
    TEST_ASSERT_EQUAL( 1, isIllegal(222222) );
    TEST_ASSERT_EQUAL( 1, isIllegal(446446) );
    TEST_ASSERT_EQUAL( 1, isIllegal(38593859) );
}

void test_range() {
    // Edge case: 'begin' is greater than 'end'
    TEST_ASSERT_EQUAL( 0, nextIllegal(23, 22));

    TEST_ASSERT_EQUAL( 11, nextIllegal(11, 22));
    TEST_ASSERT_EQUAL( 22, nextIllegal(12, 22));

    TEST_ASSERT_EQUAL( 99, nextIllegal(95, 115));
    TEST_ASSERT_EQUAL(  0, nextIllegal(100, 115));

    TEST_ASSERT_EQUAL( 1188511885, nextIllegal(1188511880, 1188511890));
    TEST_ASSERT_EQUAL( 0, nextIllegal(1188511886, 1188511890));

    TEST_ASSERT_EQUAL( 222222, nextIllegal(222220, 222224));
    TEST_ASSERT_EQUAL( 0, nextIllegal(222223, 222224));

    TEST_ASSERT_EQUAL( 0, nextIllegal(1698522, 1698528));

    TEST_ASSERT_EQUAL( 446446, nextIllegal(446443, 446449));
    TEST_ASSERT_EQUAL( 0, nextIllegal(446447, 446449));

    TEST_ASSERT_EQUAL( 38593859, nextIllegal(38593856, 38593862));
    TEST_ASSERT_EQUAL( 0, nextIllegal(38593860, 38593862));

    TEST_ASSERT_EQUAL( 38593859, nextIllegal(38593856, 38593862));
    TEST_ASSERT_EQUAL( 0, nextIllegal(38593860, 38593862));
}

void test_digit() {
    ull num = 123456789123456789;

    TEST_ASSERT_EQUAL( 9, digit(num, 0) );
    TEST_ASSERT_EQUAL( 8, digit(num, 1) );
    TEST_ASSERT_EQUAL( 7, digit(num, 2) );
    TEST_ASSERT_EQUAL( 6, digit(num, 3) );
    TEST_ASSERT_EQUAL( 5, digit(num, 4) );
    TEST_ASSERT_EQUAL( 4, digit(num, 5) );
    TEST_ASSERT_EQUAL( 3, digit(num, 6) );
    TEST_ASSERT_EQUAL( 2, digit(num, 7) );
    TEST_ASSERT_EQUAL( 1, digit(num, 8) );
    TEST_ASSERT_EQUAL( 9, digit(num, 9) );
    TEST_ASSERT_EQUAL( 8, digit(num, 10) );
    TEST_ASSERT_EQUAL( 7, digit(num, 11) );
    TEST_ASSERT_EQUAL( 6, digit(num, 12) );
    TEST_ASSERT_EQUAL( 5, digit(num, 13) );
    TEST_ASSERT_EQUAL( 4, digit(num, 14) );
    TEST_ASSERT_EQUAL( 3, digit(num, 15) );
    TEST_ASSERT_EQUAL( 2, digit(num, 16) );
}                      

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_1);
    RUN_TEST(test_additional);
    RUN_TEST(test_range);
    RUN_TEST(test_digit);

    return UNITY_END();
}
