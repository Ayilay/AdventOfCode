#include <string.h>
#include "unity.h"
#include "problem_solver.h"

// Slightly modified w/o linked-list
typedef struct {
    ull begin;
    ull end;
} range;

// Defined in the problem source file
extern int isIntersecting( range* r1, range* r2 );
extern void mergeIntersecting( range* r1, range* r2, range* out );

void setUp(void) {
}

void tearDown(void) {
}

//--------------------------------------------------------------------
//  Intersecting Tests
//--------------------------------------------------------------------

void notintersect() {
    /*
     *      R1
     *  |--------|       R2
     *              |--------|
     */
    range r1 = {1, 2};
    range r2 = {4, 5};

    TEST_ASSERT_FALSE( isIntersecting(&r1, &r2) );

    r1 = (range) {1, 2};
    r2 = (range) {3, 4};
    TEST_ASSERT_FALSE( isIntersecting(&r1, &r2) );
}

void barely_intersect() {
    /*
     *      R1
     *  |--------|    R2
     *           |--------|
     */
    range r1;
    range r2;

    r1 = (range) {1, 3};
    r2 = (range) {3, 7};
    TEST_ASSERT_TRUE( isIntersecting(&r1, &r2) );

    r1 = (range) {3, 7};
    r2 = (range) {1, 3};
    TEST_ASSERT_TRUE( isIntersecting(&r1, &r2) );
}

void intersect() {
    /*
     *      R1
     *  |--------|  R2
     *         |--------|
     */
    range r1;
    range r2;

    r1 = (range) {1, 5};
    r2 = (range) {3, 7};
    TEST_ASSERT_TRUE( isIntersecting(&r1, &r2) );

    r1 = (range) {3, 7};
    r2 = (range) {1, 5};
    TEST_ASSERT_TRUE( isIntersecting(&r1, &r2) );
}

void fully_contained() {
    /*
     *      R1
     *  |-----------|
     *      |----|
     *        R2
     */
    range r1;
    range r2;

    r1 = (range) {1, 7};
    r2 = (range) {3, 5};
    TEST_ASSERT_TRUE( isIntersecting(&r1, &r2) );

    r1 = (range) {3, 5};
    r2 = (range) {1, 7};
    TEST_ASSERT_TRUE( isIntersecting(&r1, &r2) );
}

void same_range() {
    /*
     *      R1
     *  |-----------|
     *  |-----------|
     *        R2
     */
    range r1;
    range r2;

    r1 = (range) {1, 7};
    r2 = (range) {1, 7};
    TEST_ASSERT_TRUE( isIntersecting(&r1, &r2) );
}

//--------------------------------------------------------------------
//  Merge Tests
//--------------------------------------------------------------------

void merge() {
    /*
     *  |------|        r1
     *      |------|    r2
     *  |----------|    out
     */
    range r1;
    range r2;
    range out;

    r1 = (range) {1, 5};
    r2 = (range) {3, 7};

    mergeIntersecting( &r1, &r2, &out );
    TEST_ASSERT_EQUAL( out.begin, 1 );
    TEST_ASSERT_EQUAL( out.end, 7 );

    r1 = (range) {3, 7};
    r2 = (range) {1, 5};

    mergeIntersecting( &r1, &r2, &out );
    TEST_ASSERT_EQUAL( out.begin, 1 );
    TEST_ASSERT_EQUAL( out.end, 7 );
}

void merge_contained() {
    /*
     *  |----------|    r1
     *      |--|        r2
     *  |----------|    out
     */
    range r1;
    range r2;
    range out;

    r1 = (range) {1, 7};
    r2 = (range) {3, 5};

    mergeIntersecting( &r1, &r2, &out );
    TEST_ASSERT_EQUAL( out.begin, 1 );
    TEST_ASSERT_EQUAL( out.end, 7 );

    r1 = (range) {3, 5};
    r2 = (range) {1, 7};

    mergeIntersecting( &r1, &r2, &out );
    TEST_ASSERT_EQUAL( out.begin, 1 );
    TEST_ASSERT_EQUAL( out.end, 7 );
}

void merge_same_memory() {
    /*
     *  Assert that it's safe to pass 'r1' or 'r2' as arg to 'out'
     *
     *  |------|        r1
     *      |------|    r2
     *  |----------|    r1
     */
    range r1;
    range r2;

    // memset to ensure no "ghost values"
    memset( &r1, 0, sizeof(range) );
    memset( &r2, 0, sizeof(range) );
    r1 = (range) {1, 7};
    r2 = (range) {3, 5};
    mergeIntersecting( &r1, &r2, &r1 );
    TEST_ASSERT_EQUAL( r1.begin, 1 );
    TEST_ASSERT_EQUAL( r1.end, 7 );

    memset( &r1, 0, sizeof(range) );
    memset( &r2, 0, sizeof(range) );
    r1 = (range) {3, 5};
    r2 = (range) {1, 7};
    mergeIntersecting( &r1, &r2, &r1 );
    TEST_ASSERT_EQUAL( r1.begin, 1 );
    TEST_ASSERT_EQUAL( r1.end, 7 );

    // Same tests but with r2 as arg to out
    memset( &r1, 0, sizeof(range) );
    memset( &r2, 0, sizeof(range) );
    r1 = (range) {1, 7};
    r2 = (range) {3, 5};
    mergeIntersecting( &r1, &r2, &r2 );
    TEST_ASSERT_EQUAL( r2.begin, 1 );
    TEST_ASSERT_EQUAL( r2.end, 7 );

    memset( &r1, 0, sizeof(range) );
    memset( &r2, 0, sizeof(range) );
    r1 = (range) {3, 5};
    r2 = (range) {1, 7};
    mergeIntersecting( &r1, &r2, &r2 );
    TEST_ASSERT_EQUAL( r2.begin, 1 );
    TEST_ASSERT_EQUAL( r2.end, 7 );
}

int main()
{
    UNITY_BEGIN();

    // Range Tests
    RUN_TEST(barely_intersect);
    RUN_TEST(notintersect);
    RUN_TEST(intersect);
    RUN_TEST(fully_contained);
    RUN_TEST(same_range);

    // Merge Tests
    RUN_TEST(merge);
    RUN_TEST(merge_contained);
    RUN_TEST(merge_same_memory);

    return UNITY_END();
}
