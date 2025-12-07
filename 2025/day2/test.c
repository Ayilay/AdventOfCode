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

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_1);
    RUN_TEST(test_additional);
    RUN_TEST(test_range);

    return UNITY_END();
}
