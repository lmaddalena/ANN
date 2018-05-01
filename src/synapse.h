#pragma once

typedef struct synapse {
    double input;                   // input value
    double weight;                  // synapses's weight
    double delta;                   // delta of the weight
    double activation;              // activation value
} t_synapse;


t_synapse *synapse_ctor(void);
void synapse_dtor(t_synapse *);