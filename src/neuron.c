#include <stdlib.h>		// malloc, free
#include <math.h>       // pow, exp, tanh, cosh
#include "neuron.h"

//
// costructor
//
t_neuron *neuron_ctor(int number_of_synapses, enum activation_functions fnc) {

    // bias
    t_synapse *bias = synapse_ctor();
    bias->input = -1;
    bias->weight = 0;

    // synapses
    t_synapse *synapses = (t_synapse*) malloc(sizeof(t_synapse) * number_of_synapses);
    
    for(int i = 0; i < number_of_synapses; i++)
        synapses[i] = *(synapse_ctor());

    // neuron
    t_neuron *neu = (t_neuron*) malloc(sizeof(t_neuron));

    neu->activation = 0;
    neu->output = 0;
    neu->delta_rule = 0;
    neu->error = 0;
    neu->beta = 1;
    neu->activation_fn = fnc;
    neu->bias = *bias;
    neu->synapses = synapses;
    neu->n_synapses = number_of_synapses;
    neu->id = 0;

	return neu;

}

//
// distructor
//
void neuron_dtor(t_neuron *neu) {
    free(neu);
}

void neuron_execute(t_neuron *neuron)
{
    double activation = 0;

    t_synapse *synapses = neuron->synapses;


    // synapses' activation
    for(int i = 0; i < neuron->n_synapses; i++)
    {
        t_synapse *syn = &synapses[i];
        syn->activation = syn->input * syn->weight;
        activation += syn->activation;
    }

    // bias' activation (treshold)
    neuron->bias.activation = neuron->bias.input * neuron->bias.weight;
    activation += neuron->bias.activation;

    // neuron's activation
    neuron->activation = activation;

    switch(neuron->activation_fn)
    {
        case SIGMOID:
            neuron->output = fn_sigmoid(neuron->activation * neuron->beta);
            break;
        case GAUSSIAN:
            neuron->output = fn_gaussian(neuron->activation * neuron->beta);
            break;
        case HTAN:
            neuron->output = fn_htan(neuron->activation * neuron->beta);
            break;
        case STEP:
            neuron->output = fn_step(neuron->activation * neuron->beta, neuron->bias.activation);
            break;
        case BIPOLAR:
            neuron->output = fn_bipolar(neuron->activation * neuron->beta, neuron->bias.activation);
            break;
        case LINEAR:
            neuron->output = neuron->activation * neuron->beta;
            break;

    }
}

void neuron_apply_delta_rule(t_neuron *neuron) {

    switch(neuron->activation_fn)
    {
        case SIGMOID :
            neuron->delta_rule = neuron->error * fn_dsigmoid(neuron->activation * neuron->beta);
            break;
        case GAUSSIAN :
            neuron->delta_rule = neuron->error * fn_dgaussian(neuron->activation * neuron->beta);
            break;
        case HTAN :
            neuron->delta_rule = neuron->error * fn_dhtan(neuron->activation * neuron->beta);
            break;
        case LINEAR:
            neuron->delta_rule = neuron->error * neuron->beta;
            break;
        default :
            neuron->delta_rule = 0;
            break;

    }

}

//
// sigmoid function
//
double fn_sigmoid(double x) {
    double y;
    //y = 1 / (1 + pow(E, -x));
    y = 1 / (1 + exp(-x));
    return y;
}

//
// derivative of the sigmoid function
//
double fn_dsigmoid(double x) {

    double y;

    // sigmoid function
    y = fn_sigmoid(x);

    // derivative of the sigmoid function
    return y * (1 - y);
}

//
// gaussian function
//
double fn_gaussian(double x) {

    // f(x) = e^(-x^2)

    double y;
    //y = pow(E, -pow(x, 2));
    y = exp(-pow(x, 2));
    return y;
    
}

//
// derivative of the gaussian function
//
double fn_dgaussian(double x) {

    //f'(x)= -2x * e^(-x^2)
    double y;
    //y = -2 * x * pow(E, -pow(x, 2));
    y = -2 * x * exp(-pow(x, 2));
    return y;
    
}

//
// hyperbolic tangent function
//
double fn_htan(double x){

    // f(x) = tanh(x) = (e^2x-1)/(e^2x+1)
    // f(x) [-1;1]
    double y;
    y = tanh(x);
    return y;
    
}

//
// derivative of the hyperbolic tangent function
//
double fn_dhtan(double x){

    // f'(x) = 1/cosh(x)^2 = beta(1-f(x)^2)
    // f'(x) [0;2]
    double y;
    y = 1/pow(cosh(x), 2);
    return y;    
}

//
// step function
//
double fn_step(double x, double treshold) {

    if (x > treshold)
        return 1;
    else
        return 0;
    
}

//
// bipolar function
//
double fn_bipolar(double x, double treshold) {

    if (x > treshold)
        return 1;
    else
        return -1;
    
}
