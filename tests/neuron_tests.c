#include "test.h"
#include "neuron.h"

DEFINE_TEST(test_ctor)
{
    t_neuron *neu = neuron_ctor(3, HTAN);
    test_assert(neu != NULL, "neuron costructor error");

    return NULL;
}

DEFINE_TEST(test_dtor)
{
    t_neuron *neu = neuron_ctor(3, HTAN);
    neuron_dtor(neu);
    return NULL;
}

DEFINE_TEST(test_execute)
{
    t_neuron *neu = neuron_ctor(3, HTAN);
    
    test_assert(neu != NULL, "neuron allocation failed!");

    neu->synapses[0].input = 1.23;
    neu->synapses[0].weight = 0.1;
    neu->synapses[1].input = 0.54;
    neu->synapses[1].weight = 0.1;
    neu->synapses[2].input = 2.12;
    neu->synapses[2].weight = 0.1;
    neu->bias.input = 0.2;
    neu->bias.weight = 0.2;
    
    neuron_execute(neu);

    test_assert(neu->activation != 0.0, "neuron activation = 0.0");
    test_assert(neu->output != 0.0, "neuron output = 0.0");

    return NULL;
}

DEFINE_TEST(test_apply_delta_rule)
{
    t_neuron *neu = neuron_ctor(3, HTAN);

    neu->synapses[0].input = 1.23;
    neu->synapses[0].weight = 0.1;
    neu->synapses[1].input = 0.54;
    neu->synapses[1].weight = 0.1;
    neu->synapses[2].input = 2.12;
    neu->synapses[2].weight = 0.1;
    neu->bias.input = 0.2;
    neu->bias.weight = 0.2;

    neuron_execute(neu);
    neu->error = 0.987;
    neuron_apply_delta_rule(neu);

    test_assert(neu->delta_rule != 0.0, "neuron delta = 0.0");

    return NULL;
}

DEFINE_TEST(all_tests)
{
    test_run(test_ctor);
    test_run(test_dtor);
    test_run(test_execute);
    test_run(test_apply_delta_rule);
    return NULL;
}

TEST_SUITE(all_tests);