# ANN - Artificial Neural Network
Arificial Neural Network implemented in C Language. The library includes: synapse, neuron and error back propagatione network (ebpn).


## Build
    make

    \bin\neural.a

## Documentation

### Synapse
    typedef struct synapse {
        double input;                   // input value
        double weight;                  // synapses's weight
        double delta;                   // delta of the weight
        double activation;              // activation value
    } t_synapse;

### Neuron
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
    
### EBPN (Error Back Propagation Network)
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
	
### Tools
    double** tools_read_data_file(
        char *path,                         // file path (es.:/home/luca/data.dat)
        int *rows,                          // pointer to int where save the number of rows
        int *cols                           // pointer to int where save the number of columns
        );                                  // return array of rows*cols data

    int tools_write_data_file(
        char *path,                         // file path (es.:/home/luca/data.dat)
        int rows,                           // number of rows
        int cols,                           // number of columns
        double **data,                      // bi-dimensional array of double
        char sep,                           // separator (';', '\t')
        char *format                        // es "%3.2f"
        );                                  // return > 0 if all ok

    double** tools_malloc_matrix(
        int rows,                           // number of rows
        int cols                            // number of columns
    );                                      // return rows x columns bidimensional array
	
## Usage

### Create the Error Back Propagation Network
Create the EBPN with 3 layers (18 neurons in the input layer, 20 neurons in the hidden layers, 2 neurons int the output layer), learning rate = 0.005, momentum = 0.8, beta = 1, activationation function = HTAN (hyperbolic tangent)

    double target_rmse = .15;
    int n_layers = 3;
    int layerspec[] = { 18, 20, 2 };    
    double momentum = 0.8;
    double epsilon = .005;
    double beta = 1.0;
    enum activation_functions fnc = HTAN;

	// create the ebp network
    t_ebpn *e = ebpn_ctor(layerspec, n_layers, epsilon, momentum, beta, fnc);
	
### Training the net

    int rows; 
    int cols; 
    double **in;		
    double **out;

    // loading training data	
    in = tools_read_data_file("/data/in.dat", &rows, & cols); 
	out = tools_read_data_file("/data/out.dat", &rows, & cols); 

	// callback functions (optional)
    void training_callback (int, double);
    void end_training_callback (int, double);
	
    ebpn_train_to_target(e, in, out, rows, 20000, target_rmse, training_callback, end_training_callback, 500);
	
### The training callback functions
Example of training callback function

    void training_callback (int epochs, double rmse) 
    {
        printf("epochs %5i: rmse = %2.5f\n", epochs, rmse);
    }

    void end_training_callback (int epochs, double rmse) 
    {
        printf("\n");    
        printf("Training completed after %i epochs with rmse = %2.5g\n\n", epochs, rmse);
    }

