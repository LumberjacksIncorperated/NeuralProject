
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
    newNeuralNetwrok->sizeOfInputVector = sizeOfInputVector;
    newNeuralNetwrok->sizeOfNeuralLayer = sizeOfNeuralLayer;
    newNeuralNetwrok->numberOfNeuralLayers = numberOfNeuralLayers;
    int numberOfWeightVectors = numberOfNeuralLayers + 1;
    int squareRootOfTheMaximumSizeOfWeightVector = MAX(sizeOfInputVector, sizeOfNeuralLayer);
    int maximumSizeOfWeightVector = squareRootOfTheMaximumSizeOfWeightVector*squareRootOfTheMaximumSizeOfWeightVector;
    newNeuralNetwrok->columnsOfWeightsForEachNeuralLayer =
      _createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue(numberOfWeightVectors, maximumSizeOfWeightVector, 0.666);
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


void _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(NeuralValue* inputVector, int inputVectorSize, NeuralValue* inputWeights, NeuralValue* outputVector, int outputVectorSize) {

}

NeuralResultClassification _classificationByInterpretingNeuralValue(NeuralValue neuralValueInQuestion) {
  NeuralResultClassification resultClassification = (neuralValueInQuestion >= 0)?TrueResultClassification:FalseResultClassification;
  return resultClassification;
}


NeuralResultClassification classifyResultForInputVectorUnderNeuralNetwork(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector) {

  int sizeToMallocForWorkingVectors = MAX(theNeuralNetwork->sizeOfInputVector, theNeuralNetwork->sizeOfNeuralLayer)*sizeof(NeuralValue);
  NeuralValue* workingVectorOfInputValues = (NeuralValue*)malloc(sizeToMallocForWorkingVectors);
  NeuralValue* workingVectorOfOutputValues = (NeuralValue*)malloc(sizeToMallocForWorkingVectors);

  _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputVector, theNeuralNetwork->sizeOfInputVector, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[0], workingVectorOfOutputValues, theNeuralNetwork->sizeOfNeuralLayer);

  for (int currentNeuralLayerNumber = 1; currentNeuralLayerNumber < theNeuralNetwork->numberOfNeuralLayers; ++currentNeuralLayerNumber) {
    memcpy(workingVectorOfInputValues, workingVectorOfOutputValues, sizeToMallocForWorkingVectors);
    _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(workingVectorOfInputValues, theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[currentNeuralLayerNumber], workingVectorOfOutputValues, theNeuralNetwork->sizeOfNeuralLayer);
  }

  NeuralValue outputResultantNeuralValue;
  _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(workingVectorOfOutputValues, theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[theNeuralNetwork->numberOfNeuralLayers], &outputResultantNeuralValue, 1);

  free(workingVectorOfInputValues);
  free(workingVectorOfOutputValues);

  return _classificationByInterpretingNeuralValue(outputResultantNeuralValue);
}
