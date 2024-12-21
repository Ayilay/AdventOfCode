#include <unity.h>
#include "string.h"
#include "cpu.h"
#include "problem_solver.h"

context dut;


void setUp()
{
    memset( &dut, 0, sizeof( dut ) );
}

void tearDown()
{
    memset( &dut, 0, sizeof( dut ) );
}

void TestA()
{
    dut.C = 9;
    dut.program[dut.nProgram++] = 2;
    dut.program[dut.nProgram++] = 6;
    runProgram( &dut );

    TEST_ASSERT_EQUAL( dut.B, 1 );
}

void TestB()
{
    // 5,0,5,1,5,4 would output 0,1,2
    memset( &dut, 0, sizeof( dut ) );
    dut.A = 10;
    dut.program[dut.nProgram++] = 5;
    dut.program[dut.nProgram++] = 0;
    dut.program[dut.nProgram++] = 5;
    dut.program[dut.nProgram++] = 1;
    dut.program[dut.nProgram++] = 5;
    dut.program[dut.nProgram++] = 4;
    runProgram( &dut );

    int exp1[] = {0, 1, 2};
    TEST_ASSERT_EQUAL( 3, dut.nOut );
    TEST_ASSERT_EQUAL_INT_ARRAY( exp1, dut.output, 3);
}

void TestC()
{
    // If register A contains 2024,
    // the program 0,1,5,4,3,0 would output 4,2,5,6,7,7,7,7,3,1,0 
    // and leave 0 in register A.
    memset( &dut, 0, sizeof( dut ) );
    dut.A = 2024;
    dut.program[dut.nProgram++] = 0;
    dut.program[dut.nProgram++] = 1;
    dut.program[dut.nProgram++] = 5;
    dut.program[dut.nProgram++] = 4;
    dut.program[dut.nProgram++] = 3;
    dut.program[dut.nProgram++] = 0;
    runProgram( &dut );

    int exp[] = {4,2,5,6,7,7,7,7,3,1,0 };
    TEST_ASSERT_EQUAL( 11, dut.nOut );
    TEST_ASSERT_EQUAL_INT_ARRAY( exp, dut.output, 11);
}
void TestD()
{
    // If register B contains 29, the program 1,7 would set register B to 26.
    memset( &dut, 0, sizeof( dut ) );
    dut.B = 29;
    dut.program[dut.nProgram++] = 1;
    dut.program[dut.nProgram++] = 7;
    runProgram( &dut );

    TEST_ASSERT_EQUAL( 26, dut.B );
}

void TestE()
{
    // If register B contains 2024 and register C contains 43690,
    // the program 4,0 would set register B to 44354.
    memset( &dut, 0, sizeof( dut ) );
    dut.B = 2024;
    dut.C = 43690;
    dut.program[dut.nProgram++] = 4;
    dut.program[dut.nProgram++] = 0;
    runProgram( &dut );

    TEST_ASSERT_EQUAL( 44354, dut.B );
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(TestA);
    RUN_TEST(TestB);
    RUN_TEST(TestC);
    RUN_TEST(TestD);
    RUN_TEST(TestE);

    return UNITY_END();
}
