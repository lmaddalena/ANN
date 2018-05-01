#include "synapse.h"
#include <stdlib.h>		// malloc, free

//
// costructor
//
t_synapse *synapse_ctor(void) {

    t_synapse *syn = (t_synapse*) malloc(sizeof(t_synapse));
	syn->input = 0;
    syn->weight = 1;
    syn->delta = 0;
    syn->activation = 0;
	
	return syn;

}

//
// distructor
//
void synapse_dtor(t_synapse *syn) {
    free(syn);
}