#include <stdlib.h>
#include "unity.h"
#include "problem_solver.h"

#include "sc_list.h"

void setUp(void) {
}

void tearDown(void) {
}

typedef struct {
    int x;

    struct sc_list l;
} dummy;

void test_1() {

    struct sc_list list1;
    struct sc_list list2;
    sc_list_init( &list1 );
    sc_list_init( &list2 );

    for( int i = 0; i < 3; i++ )
    {
        dummy* elem = malloc(sizeof(dummy));
        elem->x = i;
        sc_list_init( &elem->l );
        sc_list_add_tail( &list1, &elem->l );
    }

    dummy* first;
    for( int i = 0; i < 3; i++ )
    {
        dummy* elem = malloc(sizeof(dummy));
        elem->x = i+10;
        sc_list_init( &elem->l );
        sc_list_add_tail( &list2, &elem->l );

        // Store ptr to first element; We'll join it to list1 in a bit
        if( i == 0 )
            first = elem;
    }

    // Add first element of list2 to end of list1
    sc_list_add_tail( &list1, &first->l );

    // Now pull all elements of list1
    struct sc_list *it;
    dummy* elem;

    int n = 0;
    sc_list_foreach (&list1, it) {
        n++;
    }

    TEST_ASSERT_EQUAL( n, 6 );
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_1);

    return UNITY_END();
}
