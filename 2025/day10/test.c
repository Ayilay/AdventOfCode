#include <string.h>

#include "unity.h"
#include "problem_solver.h"

#define BIT( x )    (1<<x)

typedef struct {
    ui nbuttons;
    ui buttons[ 20 ]; // Each 'button' is bitmask of which switches are toggled

    ui nlights;
    ui joltTarg[ 20 ];
} machine;
extern machine currentMachine;

void Init1() {
    memset( &currentMachine, 0, sizeof(currentMachine));

    // Machine 1 from sample
    currentMachine.nbuttons = 6;
    currentMachine.buttons[0] = (ui) BIT(3);
    currentMachine.buttons[1] = (ui) BIT(1) | BIT(3);
    currentMachine.buttons[2] = (ui) BIT(2);
    currentMachine.buttons[3] = (ui) BIT(2) | BIT(3);
    currentMachine.buttons[4] = (ui) BIT(0) | BIT(2);
    currentMachine.buttons[5] = (ui) BIT(0) | BIT(1);

    currentMachine.nlights = 4;
    currentMachine.joltTarg[0] = 3;
    currentMachine.joltTarg[1] = 5;
    currentMachine.joltTarg[2] = 4;
    currentMachine.joltTarg[3] = 7;
}

void setUp(void) {
    ResetStates();
    Init1();
}
void tearDown(void) {
}

void test_button_joltage() {
    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 0, 3 ) );

    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 1, 1 ) );
    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 1, 3 ) );

    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 2, 2 ) );

    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 3, 2 ) );
    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 3, 3 ) );

    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 4, 0 ) );
    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 4, 2 ) );

    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 5, 0 ) );
    TEST_ASSERT_TRUE( ButtonModifiesJoltage( 5, 1 ) );

    TEST_ASSERT_FALSE( ButtonModifiesJoltage( 0, 0 ) );
    TEST_ASSERT_FALSE( ButtonModifiesJoltage( 0, 1 ) );
    TEST_ASSERT_FALSE( ButtonModifiesJoltage( 0, 2 ) );
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_button_joltage);
    return UNITY_END();
}
