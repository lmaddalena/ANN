#pragma once
#include "synapse.h"
#include "neuron.h"

typedef struct ebpn {
    t_neuron **layers;                              // layers of the network
    double epsilon;                                 // learning rate (usually from 0.1 to 0.9)
    double momentum;                                // momentum constant
    double beta;                                    // beta constant
    double rmse;                                    // root mean square error after last training age
    enum activation_functions activation_fn;        // activation function    
    int n_layers;                                   // number of layers
    int *layerspec;                                 // layer specification (ex. { 2, 4, 4, 3})
} t_ebpn;



t_ebpn *ebpn_ctor(
    int *layerspec,                                 // layer specification (ex. array {2,4,4,2})
    int n_layers,                                   // number of layers
    double epsilon,                                 // learning rate
    double momentum,                                // momentum constant
    double beta,                                    // beta constant
    enum activation_functions                       // activation function
    );

void ebpn_dtor(t_ebpn *);

int *ebpn_parselayerspec(
    char *,                                         // layer specification ex. "2:4:4:2" 
    int *                                           // number of layers
    );      

void ebpn_execute(
    t_ebpn *e,                                      // ebpn to be executed
    double *in                                      // input values
    );

void ebpn_initialize(t_ebpn *);                     // initialize weight [0, 1]

double ebpn_train(
    t_ebpn *e,                                      // ebpn to train
    double **in,                                    // input values
    double **out,                                   // output values
    int rows                                        // number of rows (data set length)
    );                                              // return the Root Mean Square Error before weight calculation

int ebpn_train_to_target(
    t_ebpn *e,                                      // ebpn to train
    double **in,                                    // input values
    double **out,                                   // output values
    int rows,                                       // number of rows (data set length)
    int max_epochs,                                 // max epochs
    double target_rmse,                             // target rmse
    void(*training_callback)(                       // training callback function
            int epochs,                             // epochs
            double rmse),                           // Root Mean Square Error   
    void(*end_training_callback)(                   // end training callback function
            int epochs,                             // epochs
            double rmse),                           // Root Mean Square Error
    int                                             // batch size
    );                                              // return number of epochs    

void ebpn_print(t_ebpn *);                          // print the net