#include <stdio.h>
#include <stdlib.h>
#include "ebpn.h"
#include "neuron.h"
#include "tools.h"

int main(){

    int layerspec[] = {1, 2, 1};    
    int n_layers = 3;

    t_ebpn *e = ebpn_ctor(layerspec, n_layers, .6, .8, 1, SIGMOID);

    double **in = malloc(1 * sizeof(*in));
    double **out = malloc(1 * sizeof(*out));
    in[0] = malloc(1 * sizeof(double));
    out[0] = malloc(1 * sizeof(double));

    in[0][0] = .5;
    out[0][0] = .5;
    ebpn_train(e, in, out, 1);

    ebpn_execute(e, in[0]);


    ebpn_print(e);

    //printf("Press [ENTER] to quit!\n");
    //getchar();

    void test_sin();
    test_sin();

    printf("\n\n");

    return 0;
    
}

void test_sin()
{
    int layerspec[] = {1, 4, 4, 1};    
    int n_layers = 4;
    int n_inputs = layerspec[0];
    int n_outputs = layerspec[n_layers - 1];

    t_ebpn *e = ebpn_ctor(layerspec, n_layers, .6, .8, 1, GAUSSIAN);


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

    ebpn_train_to_target(e, in, out, rows, 50000, .009, training_callback, NULL, 500);
    
    ebpn_dtor(e);
    
}

void training_callback (int epochs, double rmse) 
{
    if(epochs % 100 == 0)
        printf("epochs: %i; rmse: %g\r", epochs, rmse);
}
