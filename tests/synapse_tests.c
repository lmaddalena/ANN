#include "test.h"
#include "synapse.h"

DEFINE_TEST(test_ctor)
{
    t_synapse *syn = synapse_ctor();

    test_assert(syn != NULL, "synapse allocation failed!");

    syn->input = 1.2;
    syn->weight = 2.3;
    syn->delta = 3.4;
    syn->activation = 4.5;    

    test_assert(syn->input == 1.2, "synapse input error");
    test_assert(syn->weight == 2.3, "synapse weight error");
    test_assert(syn->delta == 3.4, "synapse delta error");
    test_assert(syn->activation == 4.5, "synapse activation error");

    return NULL;
}

DEFINE_TEST(test_dtor)
{
    t_synapse *syn = synapse_ctor();
    synapse_dtor(syn);
    return NULL;
}

DEFINE_TEST(all_tests)
{
    test_run(test_ctor);
    test_run(test_dtor);

    return NULL;
}

TEST_SUITE(all_tests);