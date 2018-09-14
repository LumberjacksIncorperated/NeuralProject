
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "NeuralNetwork.h"


#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })


NeuralValue** _allocInputActivationArrayLayersOfNeuralNetwork(NeuralNetwork* theNeuralNetwork);
NeuralValue _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArrayForHiddenLayers);
void _propagateBackwardsThroughLayerWithOutputVectorSizeAndOutputVectorErrorValuesAndWeightVectorAndInputVectorSizeAndInputVectorActivationValuesAndInputErrorsVectorThatWeAreGoingToOutput(int outputVectorSize, NeuralValue* outputErrorVector, NeuralValue* weightVector, int inputVectorSize, NeuralValue* inputActivationValuesVector, NeuralValue* inputErrorsVectorThatWeAreGoingToOutput);
NeuralValue _determineErrorOfNeuralValueWhenComparedToExpectedResultClassification(NeuralValue neuralValueInQuestion, NeuralResultClassification resultClassificationInQuestion);
int _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(int inputVectorIndex, int outputVectorIndex, int inputVectorSize);


void __print1dArrayWithNameArrayAndSize(char* name, NeuralValue* array, int size);
void __print2dArrayWithNameArrayAndColumnsAndRows(char* name, NeuralValue** array, int columns, int rows);



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
  NeuralValue** inputActivationArrayForHiddenLayers = _allocInputActivationArrayLayersOfNeuralNetwork(theNeuralNetwork);
  NeuralValue outputResultantNeuralValue = _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(theNeuralNetwork, inputVector, inputActivationArrayForHiddenLayers);

  int currentWeightVectorColumnIndex = theNeuralNetwork->numberOfNeuralLayers;
  int currentInputActivationColumnIndex = theNeuralNetwork->numberOfNeuralLayers;
  NeuralValue errorOfResult = _determineErrorOfNeuralValueWhenComparedToExpectedResultClassification(outputResultantNeuralValue, expectedResultClassification);
  int workingVectorsByteSize = theNeuralNetwork->sizeOfNeuralLayer*sizeof(NeuralValue);
  NeuralValue* workingInputErrorsVector_forOutput = (NeuralValue*)malloc(workingVectorsByteSize);
  NeuralValue* workingInputErrorsVector_forInput = (NeuralValue*)malloc(workingVectorsByteSize);
  _propagateBackwardsThroughLayerWithOutputVectorSizeAndOutputVectorErrorValuesAndWeightVectorAndInputVectorSizeAndInputVectorActivationValuesAndInputErrorsVectorThatWeAreGoingToOutput(1, &errorOfResult, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[currentWeightVectorColumnIndex], theNeuralNetwork->sizeOfNeuralLayer, inputActivationArrayForHiddenLayers[currentInputActivationColumnIndex], workingInputErrorsVector_forOutput);
  int numberOfSetsOfBackpropogationThoughHiddenLayers = theNeuralNetwork->numberOfNeuralLayers;
  for (int _ = 0; _ < numberOfSetsOfBackpropogationThoughHiddenLayers; ++_) {
    currentWeightVectorColumnIndex--;
    currentInputActivationColumnIndex--;
    memcpy(workingInputErrorsVector_forInput, workingInputErrorsVector_forOutput, workingVectorsByteSize);
    int theInputActivationVectorIsTheInputLayer = (currentInputActivationColumnIndex == 0);
    int sizeOfInputActivationLayer = theInputActivationVectorIsTheInputLayer?theNeuralNetwork->sizeOfInputVector:theNeuralNetwork->sizeOfNeuralLayer;
    _propagateBackwardsThroughLayerWithOutputVectorSizeAndOutputVectorErrorValuesAndWeightVectorAndInputVectorSizeAndInputVectorActivationValuesAndInputErrorsVectorThatWeAreGoingToOutput(theNeuralNetwork->sizeOfNeuralLayer, workingInputErrorsVector_forInput, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[currentWeightVectorColumnIndex], sizeOfInputActivationLayer, inputActivationArrayForHiddenLayers[currentInputActivationColumnIndex], workingInputErrorsVector_forOutput);
  }
}


void _propagateBackwardsThroughLayerWithOutputVectorSizeAndOutputVectorErrorValuesAndWeightVectorAndInputVectorSizeAndInputVectorActivationValuesAndInputErrorsVectorThatWeAreGoingToOutput(int outputVectorSize, NeuralValue* outputErrorVector, NeuralValue* weightVector, int inputVectorSize, NeuralValue* inputActivationValuesVector, NeuralValue* inputErrorsVectorThatWeAreGoingToOutput) {
  //int outputVectorSize, NeuralValue* outputErrorVector, NeuralValue* weightVector, int inputVectorSize,
  //NeuralValue* inputActivationValuesVector, NeuralValue* inputErrorsVectorThatWeAreGoingToOutput
  memset(inputErrorsVectorThatWeAreGoingToOutput, 0, sizeof(NeuralValue)*inputVectorSize);
  for (int outputVectorIndex = 0; outputVectorIndex < outputVectorSize; ++outputVectorIndex) {
      for (int inputVectorIndex = 0; inputVectorIndex < inputVectorSize; ++inputVectorIndex) {
        int inputWeightIndex = _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(inputVectorIndex, outputVectorIndex, inputVectorSize);
        NeuralValue weight = weightVector[inputWeightIndex];
        NeuralValue workingOutputErrorValue = outputErrorVector[outputVectorIndex]/(((NeuralValue)inputVectorSize));
        inputErrorsVectorThatWeAreGoingToOutput[inputVectorIndex] += workingOutputErrorValue*weight;
        NeuralValue learningRate = 0.000005;

        NeuralValue newWeightModification = inputActivationValuesVector[inputVectorIndex]*workingOutputErrorValue*learningRate;
        int theContributionOfThisLineWasPositive = (inputActivationValuesVector[inputVectorIndex]*weight > 0);
        int theErrorWasPositive = (outputErrorVector[outputVectorIndex] < 0);
        if (theContributionOfThisLineWasPositive != theErrorWasPositive) {
            newWeightModification = -newWeightModification;
        }
        weightVector[inputWeightIndex] += newWeightModification;

      }

  }
  for (int outputVectorIndex = 0; outputVectorIndex < outputVectorSize; ++outputVectorIndex) {
      for (int inputVectorIndex = 0; inputVectorIndex < inputVectorSize; ++inputVectorIndex) {
        int inputWeightIndex = _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(inputVectorIndex, outputVectorIndex, inputVectorSize);

      }

  }

}





int _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(int inputVectorIndex, int outputVectorIndex, int inputVectorSize) {
  int weightVectorIndex = inputVectorIndex + outputVectorIndex*inputVectorSize;
  return weightVectorIndex;
}

void _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(
  NeuralValue* inputVector, int inputVectorSize, NeuralValue* inputWeights, NeuralValue* outputVector, int outputVectorSize) {

    memset(outputVector, 0, sizeof(NeuralValue)*outputVectorSize);

    for (int outputVectorIndex = 0; outputVectorIndex < outputVectorSize; ++outputVectorIndex) {
        for (int inputVectorIndex = 0; inputVectorIndex < inputVectorSize; ++inputVectorIndex) {
          int inputWeightIndex = _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(inputVectorIndex, outputVectorIndex, inputVectorSize);
          NeuralValue weight = inputWeights[inputWeightIndex];
          outputVector[outputVectorIndex] += inputVector[inputVectorIndex]*weight;

        }

    }

}

NeuralResultClassification _classificationByInterpretingNeuralValue(NeuralValue neuralValueInQuestion) {
  NeuralResultClassification resultClassification = (neuralValueInQuestion >= 0)?TrueResultClassification:FalseResultClassification;
  return resultClassification;
}

NeuralValue _determineErrorOfNeuralValueWhenComparedToExpectedResultClassification(NeuralValue neuralValueInQuestion, NeuralResultClassification resultClassificationInQuestion) {
  NeuralValue theErrorIndeed = 0;

  if (resultClassificationInQuestion == TrueResultClassification) {
    if (neuralValueInQuestion < 0) theErrorIndeed = 1;//0-neuralValueInQuestion;
  } else if (resultClassificationInQuestion == FalseResultClassification) {
    if (neuralValueInQuestion >= 0) theErrorIndeed = -1;//0-neuralValueInQuestion;
  }
  return theErrorIndeed;
}


NeuralValue _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArrayForHiddenLayers) {


int currentIndexOfInputActivationArray = 0;

  for (int inputVectorIndex = 0; inputVectorIndex < theNeuralNetwork->sizeOfInputVector; ++ inputVectorIndex)
    inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray][inputVectorIndex] = inputVector[inputVectorIndex];

  currentIndexOfInputActivationArray++;
  _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputVector, theNeuralNetwork->sizeOfInputVector, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[0], inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray], theNeuralNetwork->sizeOfNeuralLayer);
  //currentIndexOfInputActivationArray++;


  for (int currentNeuralLayerNumber = 1; currentNeuralLayerNumber < theNeuralNetwork->numberOfNeuralLayers; ++currentNeuralLayerNumber) {
    _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray], theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[currentNeuralLayerNumber], inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray+1], theNeuralNetwork->sizeOfNeuralLayer);
    currentIndexOfInputActivationArray++;
  }

  NeuralValue outputResultantNeuralValue;
  _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputActivationArrayForHiddenLayers[currentIndexOfInputActivationArray], theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[theNeuralNetwork->numberOfNeuralLayers], &outputResultantNeuralValue, 1);
  return outputResultantNeuralValue;

}

NeuralValue** _allocInputActivationArrayLayersOfNeuralNetwork(NeuralNetwork* theNeuralNetwork) {
  NeuralValue** inputActivationArrayForHiddenLayers = (NeuralValue**)malloc(sizeof(NeuralValue*)*(1+theNeuralNetwork->numberOfNeuralLayers));
  for (int inputActivationLayerIndex = 0; inputActivationLayerIndex < theNeuralNetwork->numberOfNeuralLayers+1; ++inputActivationLayerIndex) {
    inputActivationArrayForHiddenLayers[inputActivationLayerIndex] = (NeuralValue*)malloc(sizeof(NeuralValue)*MAX(theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->sizeOfInputVector));
  }
  return inputActivationArrayForHiddenLayers;
}



NeuralResultClassification classifyResultForInputVectorUnderNeuralNetwork(NeuralValue* inputVector, NeuralNetwork* theNeuralNetwork) {
__print1dArrayWithNameArrayAndSize("Initial Input Vector", inputVector, theNeuralNetwork->sizeOfInputVector);

  NeuralValue** inputActivationArrayForHiddenLayers = _allocInputActivationArrayLayersOfNeuralNetwork(theNeuralNetwork);
  __print2dArrayWithNameArrayAndColumnsAndRows("Initial Input Activation Array", inputActivationArrayForHiddenLayers, 1+theNeuralNetwork->numberOfNeuralLayers, MAX(theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->sizeOfInputVector));
  NeuralValue outputResultantNeuralValue = _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(theNeuralNetwork, inputVector, inputActivationArrayForHiddenLayers);
  __print2dArrayWithNameArrayAndColumnsAndRows("Final Input Activation Array", inputActivationArrayForHiddenLayers, 1+theNeuralNetwork->numberOfNeuralLayers, MAX(theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->sizeOfInputVector));

  printf("OUTPUT VALUE: %lf\n", outputResultantNeuralValue);
  return _classificationByInterpretingNeuralValue(outputResultantNeuralValue);
}


void __print1dArrayWithNameArrayAndSize(char* name, NeuralValue* array, int size) {
  printf("\n:::%s------------------\n", name);
  for (int i = 0; i < size; ++i)
    printf(":%lf", array[i]);
  printf("\n------------------\n\n");
}
void __print2dArrayWithNameArrayAndColumnsAndRows(char* name, NeuralValue** array, int columns, int rows) {
  printf("\n:::%s------------------\n", name);
  for (int i = 0; i < columns; ++i) {
    for (int y = 0; y < rows; ++y)
      printf(":%lf", array[i][y]);
    printf("\n");
  }
  printf("------------------\n\n");
}
