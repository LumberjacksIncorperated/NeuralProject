
#include <stdlib.h>
#include <string.h>

#include "NeuralNetwork.h"


NeuralValue** _createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue(int numberOfColumns, int numberOfValuesPerColumn, NeuralValue defaultNeuralValue) {
  NeuralValue** columnsOfNeuralValues = (NeuralValue**)malloc(numberOfColumns*sizeof(NeuralValue*));
  for (int columnIndex = 0; columnIndex < numberOfColumns; ++columnIndex) {
    columnsOfNeuralValues[columnIndex] = (NeuralValue*)malloc(numberOfValuesPerColumn*sizeof(NeuralValue));
    for (int valueInColumnIndex = 0; valueInColumnIndex < numberOfValuesPerColumn; valueInColumnIndex++) {
      columnsOfNeuralValues[columnIndex][valueInColumnIndex] = defaultNeuralValue;
    }
  }
  return columnsOfNeuralValues;
}


NeuralNetwork* newNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerAndNumberOfNeuralLayers(int sizeOfInputVector, int sizeOfNeuralLayer, int numberOfNeuralLayers) {
  int inputsAreTrash = ((sizeOfInputVector <= 0) || (sizeOfNeuralLayer <= 0) || (numberOfNeuralLayers <= 0))
  NeuralNetwork* newNeuralNetwrok = NULL;
  if (!inputsAreTrash) {
    newNeuralNetwrok = malloc(sizeof(NeuralNetwork));
    newNeuralNetwrok->columnsOfWeightsForEachNeuralLayer =
      _createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue();
  }
  return newNeuralNetwrok;
}
void destroyNeuralNetwork(NeuralNetwork *neuralNetworkToDestroy) {
  if (neuralNetworkToDestroy) {
    free(neuralNetworkToDestroy->columnsOfWeightsForEachNeuralLayer);
    free(neuralNetworkToDestroy);
  }
}

void learnOnInputVectorAndExpectedResultClassification(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralResultClassification expectedResultClassification) {

}


NeuralResultClassification classifyResultForInputVectorUnderNeuralNetwork(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector) {
  return TrueResultClassification;
}
