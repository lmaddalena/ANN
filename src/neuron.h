#pragma once

#include "synapse.h"
#include <math.h>

#define E   exp(1.0)

enum activation_functions {
    SIGMOID,
    GAUSSIAN,
    HTAN,
    STEP,
    BIPOLAR,
    LINEAR
};

typedef struct neuron {
    t_synapse *synapses;                            // synapses of the neuron
    t_synapse bias;                                 // bias of the neuron (set the working point of the neuron)
    double activation;                              // activation value
    double output;                                  // neuron's output
    double delta_rule;                              // value of delta rule
    double error;                                   // error of the neuron
    double beta;                                    // beta constant
    enum activation_functions activation_fn;        // activation function
    int n_synapses;                                 // number of synapses
    int id;                                         // neuron's id
    
} t_neuron;


t_neuron *neuron_ctor(int number_of_synapses, enum activation_functions fnc);
void neuron_dtor(t_neuron *);
void neuron_execute(t_neuron *);
void neuron_apply_delta_rule(t_neuron *);

double fn_sigmoid(double);
double fn_gaussian(double);
double fn_htan(double);
double fn_step(double x, double treshold);
double fn_bipolar(double x, double treshold);

double fn_dsigmoid(double);
double fn_dgaussian(double);
double fn_dhtan(double);
