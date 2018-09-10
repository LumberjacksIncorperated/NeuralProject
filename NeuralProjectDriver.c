
#include <stdio.h>
#include <stdlib.h>

#include "NeuralNetwork.h"

int main() {
  printf("Starting the driver...\n");


  NeuralNetwork* aNeuralNetwork = newNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerAndNumberOfNeuralLayers(3, 5, 2);

  NeuralValue someInputVector[3] = {12.4, 123, -2};
  learnOnInputVectorAndExpectedResultClassification(aNeuralNetwork, someInputVector, FalseResultClassification);

  destroyNeuralNetwork(aNeuralNetwork);

  return 0;
}
