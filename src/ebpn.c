#include <stdio.h>
#include <stdlib.h>		// malloc, free, srand, rand, NULL, RAND_MAX
#include <string.h>     // strtok, strlen, strcpy
#include <stdlib.h>     // atoi
#include <ctype.h>      // isdigit
#include <time.h>       // time
#include "ebpn.h"


//
// costructor
//
t_ebpn *ebpn_ctor(int *layerspec, int n_layers, double epsilon, double momentum, double beta, enum activation_functions activation_fn) {

    int n_synapses = 0;

    // layers allocation
    t_neuron **layers = (t_neuron **) malloc(n_layers * sizeof(t_neuron *));

    // layer allocation
    for(int i = 0; i < n_layers; i++)
    {
        int n_neurons = layerspec[i];        
        layers[i] = (t_neuron *) malloc(n_neurons * sizeof(t_neuron));

        for(int j = 0; j < n_neurons; j++)
        {
            if(i == 0)      // input layer
            {
                n_synapses = 1;
                layers[i][j] = *neuron_ctor(n_synapses, LINEAR);
                
            }
            else            // other layers
            {
                n_synapses = layerspec[i -1];
                layers[i][j] = *neuron_ctor(n_synapses, activation_fn);
            }

            layers[i][j].beta = beta;
            layers[i][j].id = (i + 1) * 10 + (j + 1);
        }
    }

    // ebpn allocation
    t_ebpn *ebpn = (t_ebpn *) malloc(sizeof(t_ebpn));

    ebpn->layers = layers;
    ebpn->epsilon = epsilon;
    ebpn->momentum = momentum;
    ebpn->beta = beta;
    ebpn->rmse = 0;
    ebpn->activation_fn = activation_fn;
    ebpn->n_layers = n_layers;
    ebpn->layerspec = layerspec;

    ebpn_initialize(ebpn);

    return ebpn;
}

//
// distructor
//
void ebpn_dtor(t_ebpn *ebpn) {
    free(ebpn);
}

//
// parse layer specification
// ex.:
// int **layerspec = ebpn_parselayerspec("2:4:4:2", &n_layers);
// return {2, 4, 4, 2}, put 4 in n_layers
//
int *ebpn_parselayerspec(char *s, int *n_layers)
{
    int i = 0;
    char s2[strlen(s)];
    char *p;

    strcpy(s2, s);

    p = strtok(s, ":");
    while(p != NULL)
    {
        (*n_layers)++;
        p = strtok(NULL, ":");
    }

    int *out;
    out = malloc(sizeof(int) * (*n_layers));

    p = strtok(s2, ":");
    while(p != NULL)
    {
        out[i++] = atoi(p);
        p = strtok(NULL, ":");
    }

    return out;
}

//
// execute the net
void ebpn_execute(t_ebpn *ebpn, double *in)
{

    int n_inputs = ebpn->layerspec[0];

    // input layer
    for(int i = 0; i < n_inputs; i++)
    {
        t_neuron *neu = &ebpn->layers[0][i];
        neu->synapses[0].input = in[i];
        neuron_execute(neu);
    }

    // hidden layers
    for (int j = 1; j < ebpn->n_layers; j++)
    {
        t_neuron *curr_layer = ebpn->layers[j];
        t_neuron *prev_layer = ebpn->layers[j - 1];

        for (int k = 0; k < ebpn->layerspec[j]; k++)
        {
            t_neuron *curr_neuron = &curr_layer[k];

            // set the synapses of current neuron with the output values of prev layer
            for(int n = 0; n < ebpn->layerspec[j - 1]; n++)
                curr_neuron->synapses[n].input = prev_layer[n].output;

            // execute the neuron
            neuron_execute(curr_neuron); 

        }
    
    }
    

}

//
// initialize synapses & bias' weights (input layer escluded)
void ebpn_initialize(t_ebpn *ebpn)
{
    srand(time(NULL));
    
    for(int l = 1; l < ebpn->n_layers; l++)
    {
        // current layer
        t_neuron *layer = ebpn->layers[l];

        // nurons of the layer
        int n_neurons = ebpn->layerspec[l];

        for(int n = 0; n < n_neurons; n++)
        {
            // current neuron
            layer[n].bias.weight = (double)(rand()) / RAND_MAX;

            // number of synapses
            int n_synapses = layer[n].n_synapses;

            for(int s = 0; s < n_synapses; s++)
            {
                // current synapses
                layer[n].synapses[s].weight = (double)(rand()) / RAND_MAX;
            }

        }

    }
}

//
// train the net
double ebpn_train(t_ebpn *ebpn, double **in, double **out, int n_rows)
{
    double sqre = 0.0;          // square error
    double mse = 0.0;           // mean square error
    double rmse = 0.0;          // root mean square error

    int n_outputs = ebpn->layerspec[ebpn->n_layers - 1];

    for(int row = 0; row < n_rows; row++)
    {
        // current input values
        double *curr_input = in[row];

        // current output values
        double *curr_output = out[row];

        // execute the net
        ebpn_execute(ebpn, curr_input);

        // errors calculation
        // ( i >= 1 esclude input layer)
        for(int i = ebpn->n_layers - 1; i >= 1; i--)
        {
            // current layer
            t_neuron *curr_layer = ebpn->layers[i];

            // next layer
            t_neuron *next_layer = NULL;

            if (i < ebpn->n_layers - 1) 
                next_layer = ebpn->layers[i + 1];

            if (next_layer == NULL)
            {
                // i'm in the output layer

                for (int j = 0; j < ebpn->layerspec[i]; j++)
                {
                    t_neuron *curr_neuron = &curr_layer[j];
                    curr_neuron->error = curr_output[j] - curr_neuron->output;
                    neuron_apply_delta_rule(curr_neuron);

                    sqre += pow(curr_neuron->error, 2);
                }
            }
            else
            {
                // ...other layers

                for (int j = 0; j < ebpn->layerspec[i]; j++)
                { 
                    t_neuron *curr_neuron = &curr_layer[j];
                    curr_neuron->error = 0.0;

                    for (int k = 0; k < ebpn->layerspec[i + 1]; k++)
                    {
                        t_neuron *next_neuron = &next_layer[k];
                        t_synapse *syn = &next_neuron->synapses[j];  // synapse that connect currentNeuron with nextNeuron
                        curr_neuron->error += (next_neuron->delta_rule * syn->weight);
                    }

                    neuron_apply_delta_rule(curr_neuron);
                }

            }                        
        }

        double delta = 0.0;         
        double momentum = 0.0;      

        // alter weights (i >= 1 esclude input layer)
        for(int i = ebpn->n_layers - 1; i >= 1; i--)
        {
            // current layer
            t_neuron *curr_layer = ebpn->layers[i];

            for (int j = 0; j < ebpn->layerspec[i]; j++)
            {
                t_neuron *curr_neuron = &curr_layer[j];

                // adjust the synapses weight
                for(int k = 0; k < curr_neuron->n_synapses; k++)
                {
                    t_synapse *curr_synapse = &curr_neuron->synapses[k];
                    delta = (ebpn->epsilon * curr_neuron->delta_rule * curr_synapse->input);
                    momentum = ebpn->momentum * curr_synapse->delta;
                    curr_synapse->weight = curr_synapse->weight + delta + momentum;
                    curr_synapse->delta = delta;
                }

                // adjust the neuron's weight
                delta = (ebpn->epsilon * curr_neuron->delta_rule * curr_neuron->bias.input);
                momentum = ebpn->momentum * curr_neuron->bias.delta;
                curr_neuron->bias.weight = curr_neuron->bias.weight + delta + momentum;
                curr_neuron->bias.delta = delta;

            }
        }
        
    }


    // the Mean Square Error, MSE
    // double mse = sqre / n_outputs
    mse = sqre / (n_outputs * n_rows);

    // the Root Mean Square Error, RMSE
    rmse = sqrt(mse);

    ebpn->rmse = rmse;
    
    return rmse;
}

//
// train the net to target rmse
int ebpn_train_to_target(t_ebpn *ebpn, double **in, double **out, int n_rows, int max_epochs, double target, void (*training_callback)(int, double), void (*end_training_callback)(int, double), int batch)
{
    int i = 0;
    double rmse = 0.0;

    while (i < max_epochs)
    {
        i++;
        rmse = ebpn_train(ebpn, in, out, n_rows);

        // invoke the collback function
        if((*training_callback) != NULL && i % batch == 0)
            (*training_callback)(i, rmse);


        if(rmse <= target) 
            break;
    }

    // invoke the collback function
    if((*end_training_callback) != NULL)
        (*end_training_callback)(i, rmse);

    return i;
    
}

void ebpn_print(t_ebpn *ebpn)
{
    const char *activation_fn[] = { "SIGMOID",
                                    "GAUSSIAN",
                                    "HTAN",
                                    "STEP",
                                    "BIPOLAR",
                                    "LINEAR"  };
    
    printf("\n");

    printf("Error Back Propagation Network\n\n");
    printf("Learning Rate:\t\t%.2f\n", ebpn->epsilon);
    printf("Momentum:\t\t%.2f\n", ebpn->momentum);
    printf("Beta:\t\t\t%.2f\n", ebpn->beta);
    printf("Activation Function:\t%s\n", activation_fn[ebpn->activation_fn]);
    printf("Layers:\t\t\t%i\n", ebpn->n_layers);
    printf("Network Topolgy:\t");

    for(int i = 0; i < ebpn->n_layers; i++)
    {
        printf("%i", ebpn->layerspec[i]);
        if(i < ebpn->n_layers-1)
            printf(":");
    }
    printf("\n\n");


    for(int i = 0; i < ebpn->n_layers; i++)
    {
        t_neuron* current_layer = ebpn->layers[i];


        printf("Layer %i:\n", i+1);
        printf("------------------------------------------------------------------------------------------------------------------------\n");

        for(int j = 0; j < ebpn->layerspec[i]; j++)
        {
            t_neuron* current_neuron = &current_layer[j];
            printf("Neuron #id:\t\t%i\n", current_neuron->id);
            printf("Actiovation Function:\t%s\n", activation_fn[current_neuron->activation_fn]);
            printf("Activation:\t\t%.10f\n", current_neuron->activation);
            printf("Output:\t\t\t%.10f\n", current_neuron->output);
            printf("Delta Rule:\t\t%.10f\n", current_neuron->delta_rule);
            printf("Error:\t\t\t%.10f\n", current_neuron->error);
            printf("Beta:\t\t\t%.2f\n", current_neuron->beta);

            printf("Bias:\t\t\t");
            printf("Input: %.10f\t", current_neuron->bias.input);
            printf("Weight: %.10f\t", current_neuron->bias.weight);
            printf("Delta: %.10f\t", current_neuron->bias.delta);
            printf("Activation: %.10f\n", current_neuron->bias.activation);

            printf("Synapses:");
            for(int k = 0; k < current_neuron->n_synapses; k++)
            {
                printf("\t\t");
                t_synapse *current_syn = &current_neuron->synapses[k];
                printf("Input: %.10f\t", current_syn->input);
                printf("Weight: %.10f\t", current_syn->weight);
                printf("Delta: %.10f\t", current_syn->delta);
                printf("Activation: %.10f\n\t", current_syn->activation);

            }
            printf("\n");
        }

        printf("\n");
    }

    printf("\n");
}
