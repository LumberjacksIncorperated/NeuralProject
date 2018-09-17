#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H
//-----------------------------------------------------------------------------------------
// AUTHOR
// ------
// Lumberjacks Incorperated (2018)
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-----------------------------------------------------------------------------------------

/*
   A definition for the type of values used as inputs into the neural network used in this
   interface
*/
typedef double NeuralValue;

/*
   This struct used to maintain a neural network in this neural network interface
*/
typedef struct _NeuralNetwork {
  int sizeOfInputVector;
  int sizeOfNeuralLayer;
  int numberOfNeuralLayers;
  NeuralValue** columnsOfWeightsForEachNeuralLayer;
} NeuralNetwork;

/*
   An enumeration used for the possible output classifications of this neural network
   interface
*/
typedef enum _NeuralResultClassification {TrueResultClassification, FalseResultClassification} NeuralResultClassification;

//-----------------------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------------------

/*
   This function is used to to create a neural network based on certain given input specifications

   sizeOfInputVector
      - chooses the exact input vector size the resulting neural network will be able to peform classification and learning on
   sizeOfNeuralLayer
      - chooses the number of "neurons" in each of the hidden layers inside of the neural network
   numberOfNeuralLayers
      - chooses the number of hidden layers for the neural network

   Returns
      - a neural network structure that conforms to the above input parameters
*/
NeuralNetwork* newNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerAndNumberOfNeuralLayers(int sizeOfInputVector, int sizeOfNeuralLayer, int numberOfNeuralLayers);

/*
   This function is used to destroy and free up the associated resources of a previously allocated neural network from this interface

   neuralNetworkToDestroy
      - the previously created neural network that will be destroyed by this function
*/
void destroyNeuralNetwork(NeuralNetwork *neuralNetworkToDestroy);

/*
   This function is used to "learn" on the given neural network, using an input vector, and an expected result classification of that input vector

   theNeuralNetwork
      - The neural network that will be trasnformed by the learning
   inputVector
      - The input vector for the learning stage
   NeuralResultClassification
      - The result that is expected if the neural network was perfectly learned on the input vector
*/
void learnOnInputVectorAndExpectedResultClassification(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralResultClassification expectedResultClassification);

/*
   This function is used to create a result classification of a given input vector propogated through a given neural network

   inputVector
      - The input vector is the set of inputs propogated through the neural network
   theNeuralNetwork
      - The is the neural network the above set of inputs is propogated through

   Returns
      - A result classification of the input vector passed through the given neural network
*/
NeuralResultClassification classifyResultForInputVectorUnderNeuralNetwork(NeuralValue* inputVector, NeuralNetwork* theNeuralNetwork);

//-----------------------------------------------------------------------------------------
// TEST DEFINITIONS
//-----------------------------------------------------------------------------------------
void test_module_neural_network();

#endif
