#include "unity.h"
#include "problem_solver.h"

void setUp(void) {
}

void tearDown(void) {
}

extern ull CountArrangements( char* bluep, int* constraints, int nconstr );
extern int ContiguousNonBlank( const char* str );
extern int ContiguousHash( const char* str );

void test_trivial() {
    TEST_ASSERT_EQUAL( 0, CountArrangements("", 0, 0));
}

void test_cont_nonblank() {
    TEST_ASSERT_EQUAL( 0, ContiguousNonBlank( "" ));
    TEST_ASSERT_EQUAL( 0, ContiguousNonBlank( "." ));
    TEST_ASSERT_EQUAL( 0, ContiguousNonBlank( "....." ));

    TEST_ASSERT_EQUAL( 0, ContiguousNonBlank( ".#" ));
    TEST_ASSERT_EQUAL( 0, ContiguousNonBlank( ".?" ));

    TEST_ASSERT_EQUAL( 1, ContiguousNonBlank( "#" ));
    TEST_ASSERT_EQUAL( 1, ContiguousNonBlank( "?" ));

    TEST_ASSERT_EQUAL( 3, ContiguousNonBlank( "###" ));
    TEST_ASSERT_EQUAL( 3, ContiguousNonBlank( "###." ));
    TEST_ASSERT_EQUAL( 3, ContiguousNonBlank( "###..#" ));

    TEST_ASSERT_EQUAL( 3, ContiguousNonBlank( "???" ));
    TEST_ASSERT_EQUAL( 3, ContiguousNonBlank( "???." ));
    TEST_ASSERT_EQUAL( 3, ContiguousNonBlank( "???..?" ));
}

void test_cont_hash() {
    TEST_ASSERT_EQUAL( 0, ContiguousHash( "" ));
    TEST_ASSERT_EQUAL( 0, ContiguousHash( "." ));
    TEST_ASSERT_EQUAL( 0, ContiguousHash( "....." ));

    TEST_ASSERT_EQUAL( 0, ContiguousHash( ".#" ));
    TEST_ASSERT_EQUAL( 0, ContiguousHash( ".?" ));

    TEST_ASSERT_EQUAL( 1, ContiguousHash( "#" ));
    TEST_ASSERT_EQUAL( 0, ContiguousHash( "?" ));

    TEST_ASSERT_EQUAL( 3, ContiguousHash( "###" ));
    TEST_ASSERT_EQUAL( 3, ContiguousHash( "###." ));
    TEST_ASSERT_EQUAL( 3, ContiguousHash( "###..#" ));

    TEST_ASSERT_EQUAL( 1, ContiguousHash( "#?#" ));
    TEST_ASSERT_EQUAL( 0, ContiguousHash( "??#" ));
    TEST_ASSERT_EQUAL( 2, ContiguousHash( "##?..#" ));
}

void test_one_constr() {
    int constr[] = {1};

    TEST_ASSERT_EQUAL( 0, CountArrangements("", constr, 1));

    TEST_ASSERT_EQUAL( 1, CountArrangements("#", constr, 1));
    TEST_ASSERT_EQUAL( 1, CountArrangements("?", constr, 1));

    TEST_ASSERT_EQUAL( 0, CountArrangements("##", constr, 1));

    TEST_ASSERT_EQUAL( 1, CountArrangements("#?", constr, 1));
    TEST_ASSERT_EQUAL( 1, CountArrangements("?#", constr, 1));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_trivial);
    RUN_TEST(test_one_constr);
    RUN_TEST(test_cont_nonblank);
    RUN_TEST(test_cont_hash);

    return UNITY_END();
}
