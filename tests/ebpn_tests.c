#include "test.h"
#include "ebpn.h"

DEFINE_TEST(test_ctor)
{
    int layerspec[] = {2, 4, 5, 3};

    t_ebpn *e = ebpn_ctor(layerspec, 4, .5, .8, 1, HTAN);

    test_assert(e != NULL, "ebpn allocation failed!");

    test_assert(e->n_layers == 4, "number of layers");
    test_assert(e->layers != NULL, "layers is NULL");

    for(int i = 0; i < e->n_layers; i++)
    {
        for(int j = 0; j < e->layerspec[i]; j++)
        {
            // current neuron
            t_neuron neu = e->layers[i][j];

            if(i == 0)
            {
                test_assert(neu.bias.weight == 0, "neuron bias weight must be 0 in input neuron");
                test_assert(neu.n_synapses == 1, "invalid number of synapses in input neuron");
            }
            else
            {
                test_assert(neu.bias.weight != 0, "neuron bias weight 0");
                test_assert(neu.n_synapses == layerspec[i-1], "invalid number of synapses");

            }
            
            for(int k = 0; k < neu.n_synapses; k++)
            {
                // current synapses
                if(i == 0)
                    test_assert(neu.synapses[k].weight == 1, "synapses weight of input neuron must be 1");    
                else
                {
                    test_assert(neu.synapses[k].weight != 0, "synapses weight must be different from 0");
                    test_assert(neu.synapses[k].weight != 1, "synapses weight must be different from 1");                    
                }

            }

        }
    }

    return NULL;
}
DEFINE_TEST(test_parselayerspec)
{
    int *layerspec;
    int n_layers = 0;
    char s[] = "2:4:5:3";

    layerspec = ebpn_parselayerspec(s, &n_layers);

    test_assert(n_layers == 4, "number of layers");
    test_assert(layerspec[0] == 2, "layerspec [0] error");
    test_assert(layerspec[1] == 4, "layerspec [1] error");
    test_assert(layerspec[2] == 5, "layerspec [2] error");
    test_assert(layerspec[3] == 3, "layerspec [3] error");

    return NULL;
}

DEFINE_TEST(test_execute)
{
    int layerspec[] = {2, 4, 5, 1};
    t_ebpn *e = ebpn_ctor(layerspec, 4, .5, .8, 1, HTAN);

    double in[] = { 1, 2, 3, 4, 5 };

    ebpn_execute(e, in);

    return NULL;
}

DEFINE_TEST(test_train)
{
    int layerspec[] = {1, 4, 4, 1};    
    int n_layers = 4;
    int n_inputs = layerspec[0];
    int n_outputs = layerspec[n_layers - 1];

    t_ebpn *e = ebpn_ctor(layerspec, n_layers, .6, .8, 1, SIGMOID);


    double dat_in[] = { 0.0000, 0.0611, 0.1250, 0.1861, 0.2500, 0.3111,
                        0.3750, 0.4361, 0.5000, 0.5611, 0.6250, 0.6861,
                        0.7500, 0.8111, 0.8750, 0.9361, 1.0000 };

    double dat_out[] = { 0.5000, 0.6498, 0.7828, 0.8682, 0.9000, 0.8709,
                         0.7828, 0.6563, 0.5000, 0.3502, 0.2172, 0.1318,
                         0.1000, 0.1291, 0.2172, 0.3437, 0.5000 };

    int rows = 17;
    double **in = malloc(rows * sizeof(*in));
    double **out = malloc(rows * sizeof(*out));

    for(int i = 0; i < rows; i++)
    {
        in[i] = malloc(n_inputs * sizeof(double));
        out[i] = malloc(n_outputs * sizeof(double));

        // input
        for(int n = 0; n < n_inputs; n++)
        {
            in[i][n] = dat_in[i];
        }

        // output
        for(int n = 0; n < n_outputs; n++)
        {
            out[i][n] = dat_out[i];
        }

    }

    void training_callback (int, double);
    void end_training_callback (int, double);

    ebpn_train_to_target(e, in, out, rows, 8000, .009, training_callback, end_training_callback, 500);
    
    test_assert(e->rmse <= .009, "the net has not reached the expected rsme (.009)");

    ebpn_dtor(e);

    return NULL;
}

void training_callback (int epochs, double rmse) 
{
    printf("epochs: %i; rmse: %g\n", epochs, rmse);
}

void end_training_callback (int epochs, double rmse) 
{
    printf("End Training. Epochs: %i; rmse: %g\n", epochs, rmse);
}

DEFINE_TEST(test_dtor)
{
    int layerspec[] = {2, 4, 5, 3};    
    t_ebpn *e = ebpn_ctor(layerspec, 4, .5, .8, 1, HTAN);
    ebpn_dtor(e);
    return NULL;
}

DEFINE_TEST(all_tests)
{
    test_run(test_ctor);
    test_run(test_parselayerspec);
    test_run(test_execute);
    test_run(test_train);
    test_run(test_dtor);

    return NULL;
}

TEST_SUITE(all_tests);