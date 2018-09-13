

#define TRUE (0==0)
#define FALSE (!TRUE)

typedef double NeuralValue;

typedef struct _NeuralNetwork {
  int sizeOfInputVector;
  int sizeOfNeuralLayer;
  int numberOfNeuralLayers;
  NeuralValue** columnsOfWeightsForEachNeuralLayer;
} NeuralNetwork;



typedef enum _NeuralResultClassification {TrueResultClassification, FalseResultClassification} NeuralResultClassification;


NeuralNetwork* newNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerAndNumberOfNeuralLayers(int sizeOfInputVector, int sizeOfNeuralLayer, int numberOfNeuralLayers);
void destroyNeuralNetwork(NeuralNetwork *neuralNetworkToDestroy);

void learnOnInputVectorAndExpectedResultClassification(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralResultClassification expectedResultClassification);
NeuralResultClassification classifyResultForInputVectorUnderNeuralNetwork(NeuralValue* inputVector, NeuralNetwork* theNeuralNetwork);
