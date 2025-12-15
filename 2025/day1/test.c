#include "unity.h"
#include "problem_solver.h"

int dial = 50;

extern int RotateR( int* dial, int amount );
extern int RotateL( int* dial, int amount );

void setUp(void) {
    dial = 50;
}

void tearDown(void) {
}

void init() {
    TEST_ASSERT_EQUAL( 50, dial );
}

///----------------------------------------------------------------------
///  Left Rotations
///----------------------------------------------------------------------

void L20() {
    TEST_ASSERT_EQUAL( 0, RotateL(&dial, 20) );
    TEST_ASSERT_EQUAL( 30, dial );
}

void L50() {
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 50) );
    TEST_ASSERT_EQUAL( 0, dial );
}

void L100() {
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 100) );
    TEST_ASSERT_EQUAL( 50, dial );
}

void L101() {
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 101) );
    TEST_ASSERT_EQUAL( 49, dial );
}

void L200() {
    TEST_ASSERT_EQUAL( 2, RotateL(&dial, 200) );
    TEST_ASSERT_EQUAL( 50, dial );
}

void L60() {
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 60) );
    TEST_ASSERT_EQUAL( 90, dial );
}

void L150() {
    TEST_ASSERT_EQUAL( 2, RotateL(&dial, 150) );
    TEST_ASSERT_EQUAL( 0, dial );
}

void Lthreeturns() {
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 50) );
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 100) );
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 100) );
}

void LthreeAndHalfTurns() {
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 50) );
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 100) );
    TEST_ASSERT_EQUAL( 1, RotateL(&dial, 100) );
    TEST_ASSERT_EQUAL( 0, RotateL(&dial, 50) );
    TEST_ASSERT_EQUAL( 50, dial );
}

///----------------------------------------------------------------------
///  Right Rotations
///----------------------------------------------------------------------

void R20() {
    TEST_ASSERT_EQUAL( 0, RotateR(&dial, 20) );
    TEST_ASSERT_EQUAL( 70, dial );
}

void R50() {
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 50) );
    TEST_ASSERT_EQUAL( 0, dial );
}

void R100() {
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 100) );
    TEST_ASSERT_EQUAL( 50, dial );
}

void R101() {
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 101) );
    TEST_ASSERT_EQUAL( 51, dial );
}

void R200() {
    TEST_ASSERT_EQUAL( 2, RotateR(&dial, 200) );
    TEST_ASSERT_EQUAL( 50, dial );
}

void R60() {
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 60) );
    TEST_ASSERT_EQUAL( 10, dial );
}

void R150() {
    TEST_ASSERT_EQUAL( 2, RotateR(&dial, 150) );
    TEST_ASSERT_EQUAL( 0, dial );
}

void Rthreeturns() {
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 50) );
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 100) );
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 100) );
}

void RthreeAndHalfTurns() {
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 50) );
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 100) );
    TEST_ASSERT_EQUAL( 1, RotateR(&dial, 100) );
    TEST_ASSERT_EQUAL( 0, RotateR(&dial, 50) );
    TEST_ASSERT_EQUAL( 50, dial );
}

void R175() {
    TEST_ASSERT_EQUAL( 2, RotateR(&dial, 175) );
    TEST_ASSERT_EQUAL( 25, dial );
}


void Rstage1() {
    // Edge case for bug in RotateR
    TEST_ASSERT_EQUAL( 0, RotateL(&dial, 20) );
    TEST_ASSERT_EQUAL( 30, dial );
    TEST_ASSERT_EQUAL( 0, RotateR(&dial, 30) );
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(init);
    RUN_TEST(L20);
    RUN_TEST(L50);
    RUN_TEST(L60);
    RUN_TEST(L100);
    RUN_TEST(L101);
    RUN_TEST(L200);
    RUN_TEST(L150);
    RUN_TEST(Lthreeturns);
    RUN_TEST(LthreeAndHalfTurns);

    RUN_TEST(R20);
    RUN_TEST(R50);
    RUN_TEST(R60);
    RUN_TEST(R100);
    RUN_TEST(R101);
    RUN_TEST(R200);
    RUN_TEST(R150);
    RUN_TEST(R175);
    RUN_TEST(Rthreeturns);
    RUN_TEST(RthreeAndHalfTurns);

    RUN_TEST(Rstage1);

    return UNITY_END();
}
