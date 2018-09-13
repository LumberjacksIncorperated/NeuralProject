
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "NeuralNetwork.h"


#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })


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
  int inputsAreTrash = ((sizeOfInputVector <= 0) || (sizeOfNeuralLayer <= 0) || (numberOfNeuralLayers <= 0));
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





void _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(
  NeuralValue* inputVector, int inputVectorSize, NeuralValue* inputWeights, NeuralValue* outputVector, int outputVectorSize) {

    memset(outputVector, 0, sizeof(NeuralValue)*outputVectorSize);

    for (int outputVectorIndex = 0; outputVectorIndex < outputVectorSize; ++outputVectorIndex) {
        for (int inputVectorIndex = 0; inputVectorIndex < inputVectorSize; ++inputVectorIndex) {
          int inputWeightIndex = inputVectorIndex + outputVectorIndex*inputVectorSize;
          NeuralValue weight = inputWeights[inputWeightIndex];
          outputVector[outputVectorIndex] += inputVector[inputVectorIndex]*weight;
        }
    }
}

NeuralResultClassification _classificationByInterpretingNeuralValue(NeuralValue neuralValueInQuestion) {
  NeuralResultClassification resultClassification = (neuralValueInQuestion >= 0)?TrueResultClassification:FalseResultClassification;
  return resultClassification;
}


NeuralValue _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArrayForHiddenLayers) {
  //int sizeToMallocForWorkingVectors = MAX(theNeuralNetwork->sizeOfInputVector, theNeuralNetwork->sizeOfNeuralLayer)*sizeof(NeuralValue);
  //NeuralValue* workingVectorOfInputValues = (NeuralValue*)malloc(sizeToMallocForWorkingVectors);
  //NeuralValue* workingVectorOfOutputValues = (NeuralValue*)malloc(sizeToMallocForWorkingVectors);



  int currentIndexOfInputActivationArray = 0;
  _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputVector, theNeuralNetwork->sizeOfInputVector, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[0], inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray], theNeuralNetwork->sizeOfNeuralLayer);

  for (int currentNeuralLayerNumber = 1; currentNeuralLayerNumber < theNeuralNetwork->numberOfNeuralLayers; ++currentNeuralLayerNumber) {
    _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray], theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[currentNeuralLayerNumber], inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray+1], theNeuralNetwork->sizeOfNeuralLayer);
    currentIndexOfInputActivationArray++;
  }

  NeuralValue outputResultantNeuralValue;
  _propagateThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray], theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[theNeuralNetwork->numberOfNeuralLayers], &outputResultantNeuralValue, 1);

  //free(workingVectorOfInputValues);
  //free(workingVectorOfOutputValues);

  printf("%lf\n", outputResultantNeuralValue);

  return outputResultantNeuralValue;

  //return _classificationByInterpretingNeuralValue(outputResultantNeuralValue);
}

NeuralValue** _allocInputActivationArrayForHiddenLayersOfNeuralNetwork(NeuralNetwork* theNeuralNetwork) {
  NeuralValue** inputActivationArrayForHiddenLayers = (NeuralValue**)malloc(sizeof(NeuralValue*)*theNeuralNetwork->numberOfNeuralLayers);
  for (int inputActivationLayerIndex = 0; inputActivationLayerIndex < theNeuralNetwork->numberOfNeuralLayers; ++inputActivationLayerIndex) {
    inputActivationArrayForHiddenLayers[inputActivationLayerIndex] = (NeuralValue*)malloc(sizeof(NeuralValue)*theNeuralNetwork->sizeOfNeuralLayer);
  }
  return inputActivationArrayForHiddenLayers;
}

NeuralResultClassification classifyResultForInputVectorUnderNeuralNetwork(NeuralValue* inputVector, NeuralNetwork* theNeuralNetwork) {

  NeuralValue** inputActivationArrayForHiddenLayers = _allocInputActivationArrayForHiddenLayersOfNeuralNetwork(theNeuralNetwork);
  NeuralValue outputResultantNeuralValue = _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(theNeuralNetwork, inputVector, inputActivationArrayForHiddenLayers);

  return _classificationByInterpretingNeuralValue(outputResultantNeuralValue);



/*
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

  printf("%lf\n", outputResultantNeuralValue);
  return _classificationByInterpretingNeuralValue(outputResultantNeuralValue);
  */
}
