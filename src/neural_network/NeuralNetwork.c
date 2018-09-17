//-----------------------------------------------------------------------------------------
// AUTHOR
// ------
// Lumberjacks Incorperated (2018)
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "NeuralNetwork.h"

//-----------------------------------------------------------------------------------------
// MACRO DEFINITIONS
//-----------------------------------------------------------------------------------------
#define TRUE (0==0)
#define FALSE (!TRUE)
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

//-----------------------------------------------------------------------------------------
// INTERNAL FUNCTION FORWARD DEFINITIONS
//-----------------------------------------------------------------------------------------
NeuralValue** _allocInputActivationArrayLayersOfNeuralNetwork(NeuralNetwork* theNeuralNetwork);
NeuralValue _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArrayForHiddenLayers);
void _propagateBackwardsThroughLayerWithOutputVectorSizeAndOutputVectorErrorValuesAndWeightVectorAndInputVectorSizeAndInputVectorActivationValuesAndInputErrorsVectorThatWeAreGoingToOutput(int outputVectorSize, NeuralValue* outputErrorVector, NeuralValue* weightVector, int inputVectorSize, NeuralValue* inputActivationValuesVector, NeuralValue* inputErrorsVectorThatWeAreGoingToOutput);
NeuralValue _determineErrorOfNeuralValueWhenComparedToExpectedResultClassification(NeuralValue neuralValueInQuestion, NeuralResultClassification resultClassificationInQuestion);
int _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(int inputVectorIndex, int outputVectorIndex, int inputVectorSize);
NeuralValue* _createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue(int numberOfValuesPerColumn, NeuralValue defaultNeuralValue);
NeuralValue _getContributionOfInputVectorValueWithIndexAndSizeThroughCurrentLayerToOutputValueWithIndex(NeuralValue* inputVector, int inputVectorIndex, int inputVectorSize, NeuralValue* inputWeights, int outputVectorIndex);
void _setInputActivationArrayForInputVector(NeuralValue* inputVector, int sizeOfInputVector, NeuralValue** inputActivationArray);
NeuralValue _forwardPropogateFromLastHiddenLayerToOutputLayerAndReturnFinalOutputValue(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArray);
void _destroyColumnsOfNeuralNetwork(NeuralNetwork *neuralNetworkToDestroy);
NeuralValue** _createNeuralNetworkColumnsWithNumberOfNeuralLayersSizeOfInputVectorAndSizeOfNeuralLayer(int numberOfNeuralLayers, int sizeOfInputVector, int sizeOfNeuralLayer);
void _setStructValuesOfNewNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerNumberOfNeuralLayers(NeuralNetwork* neuralNetwork, int sizeOfInputVector, int sizeOfNeuralLayer, int numberOfNeuralLayers);

//-----------------------------------------------------------------------------------------
// META FUNCTION FORWARD DEFINITIONS
//-----------------------------------------------------------------------------------------
void __print1dArrayWithNameArrayAndSize(char* name, NeuralValue* array, int size);
void __print2dArrayWithNameArrayAndColumnsAndRows(char* name, NeuralValue** array, int columns, int rows);

//-----------------------------------------------------------------------------------------
// INTERNAL FUNCTIONS
//-----------------------------------------------------------------------------------------
NeuralValue* _createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue(int numberOfValuesPerColumn, NeuralValue defaultNeuralValue) {
    NeuralValue* columnOfNerualValues = (NeuralValue*) malloc(numberOfValuesPerColumn * sizeof(NeuralValue));
    for (int valueIndex = 0; valueIndex < numberOfValuesPerColumn; valueIndex++) {
        columnOfNerualValues[valueIndex] = defaultNeuralValue;
    }
    return columnOfNerualValues;
}

NeuralValue** _createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue(int numberOfColumns, int numberOfValuesPerColumn, NeuralValue defaultNeuralValue) {
    NeuralValue** columnsOfNeuralValues = (NeuralValue**) malloc(numberOfColumns * sizeof(NeuralValue*));
    for (int columnIndex = 0; columnIndex < numberOfColumns; ++columnIndex) {
        columnsOfNeuralValues[columnIndex] = _createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue(numberOfValuesPerColumn, defaultNeuralValue);
    }
    return columnsOfNeuralValues;
}

void _propagateBackwardsThroughLayerWithOutputVectorSizeAndOutputVectorErrorValuesAndWeightVectorAndInputVectorSizeAndInputVectorActivationValuesAndInputErrorsVectorThatWeAreGoingToOutput(int outputVectorSize, NeuralValue* outputErrorVector, NeuralValue* weightVector, int inputVectorSize, NeuralValue* inputActivationValuesVector, NeuralValue* inputErrorsVectorThatWeAreGoingToOutput) {

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

NeuralValue _getContributionOfInputVectorValueWithIndexAndSizeThroughCurrentLayerToOutputValueWithIndex(NeuralValue* inputVector, int inputVectorIndex, int inputVectorSize, NeuralValue* inputWeights, int outputVectorIndex) {
    int inputWeightIndex = _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(inputVectorIndex, outputVectorIndex, inputVectorSize);
    NeuralValue weight = inputWeights[inputWeightIndex];
    NeuralValue contributionOfInputVectorValueThroughNetworkToCurrentOutputValue = inputVector[inputVectorIndex]*weight;
    return contributionOfInputVectorValueThroughNetworkToCurrentOutputValue;
}

void _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(NeuralValue* inputVector, int inputVectorSize, NeuralValue* inputWeights, NeuralValue* outputVector, int outputVectorSize) {
    memset(outputVector, 0, sizeof(NeuralValue)*outputVectorSize);
    for (int outputVectorIndex = 0; outputVectorIndex < outputVectorSize; ++outputVectorIndex) {
        for (int inputVectorIndex = 0; inputVectorIndex < inputVectorSize; ++inputVectorIndex) {
            outputVector[outputVectorIndex] += _getContributionOfInputVectorValueWithIndexAndSizeThroughCurrentLayerToOutputValueWithIndex(inputVector, inputVectorIndex, inputVectorSize, inputWeights, outputVectorIndex);
        }
    }
}

NeuralResultClassification _classificationByInterpretingNeuralValue(NeuralValue neuralValueInQuestion) {
    NeuralResultClassification resultClassification = (neuralValueInQuestion >= 0) ? TrueResultClassification:FalseResultClassification;
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

#define INPUT_LAYER_OF_INPUT_ACTIVATION_MATRIX 0
void _setInputActivationArrayForInputVector(NeuralValue* inputVector, int sizeOfInputVector, NeuralValue** inputActivationArray) {
    for (int inputVectorIndex = 0; inputVectorIndex < sizeOfInputVector; ++inputVectorIndex) {
        inputActivationArray[INPUT_LAYER_OF_INPUT_ACTIVATION_MATRIX][inputVectorIndex] = inputVector[inputVectorIndex];
    }
}

#define INPUT_LAYER_OF_INPUT_ACTIVATION_MATRIX_FOR_FIRST_PROPOGATION 1
void _forwardPropogateFromInputLaterToFirstHiddenLayer(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArray) {
    _setInputActivationArrayForInputVector(inputVector, theNeuralNetwork->sizeOfInputVector, inputActivationArray);
    _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputVector, theNeuralNetwork->sizeOfInputVector, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[0], inputActivationArray[INPUT_LAYER_OF_INPUT_ACTIVATION_MATRIX_FOR_FIRST_PROPOGATION], theNeuralNetwork->sizeOfNeuralLayer);
}

void _forwardPropogateFromFirstHiddenLayerToLastHiddenLayer(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArray) {
    for (int currentNeuralLayerNumber = 1; currentNeuralLayerNumber < theNeuralNetwork->numberOfNeuralLayers; ++currentNeuralLayerNumber) {
        _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputActivationArray[currentNeuralLayerNumber], theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[currentNeuralLayerNumber], inputActivationArray[currentNeuralLayerNumber+1], theNeuralNetwork->sizeOfNeuralLayer);
    }
}

NeuralValue _forwardPropogateFromLastHiddenLayerToOutputLayerAndReturnFinalOutputValue(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArray) {
    NeuralValue outputResultantNeuralValue;
    _propagateForwardThroughOneNeuralLayerWithInputVectorAndInputVectorSizeAndInputWeightsAndOutputVectorAndOutputVectorSize(inputActivationArray[theNeuralNetwork->numberOfNeuralLayers], theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->columnsOfWeightsForEachNeuralLayer[theNeuralNetwork->numberOfNeuralLayers], &outputResultantNeuralValue, 1);
    return outputResultantNeuralValue;
}

NeuralValue _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(NeuralNetwork* theNeuralNetwork, NeuralValue* inputVector, NeuralValue** inputActivationArray) {
    _forwardPropogateFromInputLaterToFirstHiddenLayer(theNeuralNetwork, inputVector, inputActivationArray);
    _forwardPropogateFromFirstHiddenLayerToLastHiddenLayer(theNeuralNetwork, inputVector, inputActivationArray);
    NeuralValue outputResultantNeuralValue = _forwardPropogateFromLastHiddenLayerToOutputLayerAndReturnFinalOutputValue(theNeuralNetwork, inputVector, inputActivationArray);
    return outputResultantNeuralValue;
}

NeuralValue** _allocInputActivationArrayLayersOfNeuralNetwork(NeuralNetwork* theNeuralNetwork) {
    NeuralValue** inputActivationArrayForHiddenLayers = (NeuralValue**) malloc( sizeof(NeuralValue*) * (1+theNeuralNetwork->numberOfNeuralLayers) );
    for (int inputActivationLayerIndex = 0; inputActivationLayerIndex < theNeuralNetwork->numberOfNeuralLayers+1; ++inputActivationLayerIndex) {
        inputActivationArrayForHiddenLayers[inputActivationLayerIndex] = (NeuralValue*) malloc( sizeof(NeuralValue) * MAX(theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->sizeOfInputVector));
    }
    return inputActivationArrayForHiddenLayers;
}

void _destroyColumnsOfNeuralNetwork(NeuralNetwork *neuralNetworkToDestroy) {
    for(int columnIndex = 0; columnIndex < neuralNetworkToDestroy->numberOfNeuralLayers; columnIndex++) {
        free(neuralNetworkToDestroy->columnsOfWeightsForEachNeuralLayer[columnIndex]);
    }
    free(neuralNetworkToDestroy->columnsOfWeightsForEachNeuralLayer);
}

NeuralValue** _createNeuralNetworkColumnsWithNumberOfNeuralLayersSizeOfInputVectorAndSizeOfNeuralLayer(int numberOfNeuralLayers, int sizeOfInputVector, int sizeOfNeuralLayer) {
    int numberOfWeightVectors = numberOfNeuralLayers + 1;
    int squareRootOfTheMaximumSizeOfWeightVector = MAX(sizeOfInputVector, sizeOfNeuralLayer);
    int maximumSizeOfWeightVector = squareRootOfTheMaximumSizeOfWeightVector*squareRootOfTheMaximumSizeOfWeightVector;
    return _createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue(numberOfWeightVectors, maximumSizeOfWeightVector, 0.666);
}

void _setStructValuesOfNewNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerNumberOfNeuralLayers(NeuralNetwork* neuralNetwork, int sizeOfInputVector, int sizeOfNeuralLayer, int numberOfNeuralLayers) {
    neuralNetwork->sizeOfInputVector = sizeOfInputVector;
    neuralNetwork->sizeOfNeuralLayer = sizeOfNeuralLayer;
    neuralNetwork->numberOfNeuralLayers = numberOfNeuralLayers;
    neuralNetwork->columnsOfWeightsForEachNeuralLayer = _createNeuralNetworkColumnsWithNumberOfNeuralLayersSizeOfInputVectorAndSizeOfNeuralLayer(numberOfNeuralLayers, sizeOfInputVector, sizeOfNeuralLayer);
}

//-----------------------------------------------------------------------------------------
// EXTERNAL FUNCTIONS
//-----------------------------------------------------------------------------------------
NeuralNetwork* newNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerAndNumberOfNeuralLayers(int sizeOfInputVector, int sizeOfNeuralLayer, int numberOfNeuralLayers) {
    int inputsAreTrash = ((sizeOfInputVector <= 0) || (sizeOfNeuralLayer <= 0) || (numberOfNeuralLayers <= 0));
    NeuralNetwork* newNeuralNetwork = NULL;
    if (!inputsAreTrash) {
        newNeuralNetwork = malloc(sizeof(NeuralNetwork));
        _setStructValuesOfNewNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerNumberOfNeuralLayers(newNeuralNetwork, sizeOfInputVector, sizeOfNeuralLayer, numberOfNeuralLayers);
    }
    return newNeuralNetwork;
}

void destroyNeuralNetwork(NeuralNetwork *neuralNetworkToDestroy) {
    if (neuralNetworkToDestroy) {
        if(neuralNetworkToDestroy->columnsOfWeightsForEachNeuralLayer) {
            _destroyColumnsOfNeuralNetwork(neuralNetworkToDestroy);
        }
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

NeuralResultClassification classifyResultForInputVectorUnderNeuralNetwork(NeuralValue* inputVector, NeuralNetwork* theNeuralNetwork) {
    __print1dArrayWithNameArrayAndSize("Initial Input Vector", inputVector, theNeuralNetwork->sizeOfInputVector);
    NeuralValue** inputActivationArrayForHiddenLayers = _allocInputActivationArrayLayersOfNeuralNetwork(theNeuralNetwork);
    __print2dArrayWithNameArrayAndColumnsAndRows("Initial Input Activation Array", inputActivationArrayForHiddenLayers, 1+theNeuralNetwork->numberOfNeuralLayers, MAX(theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->sizeOfInputVector));
    NeuralValue outputResultantNeuralValue = _forwardPropergateThroughNeuralNetworkWithInputVectorAndSettingInputActivationArrayAndReturningResultantValue(theNeuralNetwork, inputVector, inputActivationArrayForHiddenLayers);
    __print2dArrayWithNameArrayAndColumnsAndRows("Final Input Activation Array", inputActivationArrayForHiddenLayers, 1+theNeuralNetwork->numberOfNeuralLayers, MAX(theNeuralNetwork->sizeOfNeuralLayer, theNeuralNetwork->sizeOfInputVector));
    printf("OUTPUT VALUE: %lf\n", outputResultantNeuralValue);
    return _classificationByInterpretingNeuralValue(outputResultantNeuralValue);
}

//-----------------------------------------------------------------------------------------
// META FUNCTIONS
//-----------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------
// TEST ASSISTANCE FUNCTIONS
//-----------------------------------------------------------------------------------------
void _test_printModuleTestingMessage() {
    printf("\n----------------------------------------------------------------------------------------------\n");
    printf("\t MODULE TESTING: neural_network\n");
    printf("----------------------------------------------------------------------------------------------\n");
}

void _test_printTestNameDescriptionAndResultStub(char* testName, char* testDescription) {
    printf("\t<Test>: %s\n", testName);
    printf("\t<Decription>: %s\n", testDescription);
    printf("\t<Result>: ");
}

void _test_printTestResult(int testResult) {
    if(testResult == TRUE) {
          printf("PASSED\n\n");
    } else {
          printf("FAILED\n\n");
    }
}

//-----------------------------------------------------------------------------------------
// TEST FUNCTIONS
//-----------------------------------------------------------------------------------------
void test_getContributionOfInputVectorValueWithIndexAndSizeThroughCurrentLayerToOutputValueWithIndex() {
    _test_printTestNameDescriptionAndResultStub("getContributionOfInputVectorValueWithIndexAndSizeThroughCurrentLayerToOutputValueWithIndex", "test creates all default values");
    NeuralValue* testWeights= _createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue(25, 7);
    NeuralValue[] testInputVector = {2, 2, 2, 2, 2};
    int testInputVectorIndex = 3;
    int testOutputVectorIndex = 2;
    int testContribution = _getContributionOfInputVectorValueWithIndexAndSizeThroughCurrentLayerToOutputValueWithIndex(testInputVector, testInputVectorIndex, 5, testWeights, testOutputVectorIndex);
    printf("here %lf\n", testContribution);
    int test_result = TRUE;
    _test_printTestResult(test_result);
}

void test_mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex() {
    _test_printTestNameDescriptionAndResultStub("mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex", "test creates all default values");
    int test_result = TRUE;
    int mapTestOutputValueOne = _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(4, 4, 4);
    if(mapTestOutputValueOne != 20) test_result = FALSE;
    int mapTestOutputValueTwo = _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(4, 2, 2);
    if(mapTestOutputValueTwo != 8) test_result = FALSE;
    int mapTestOutputValueThree = _mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex(2, 3, 3);
    if(mapTestOutputValueThree != 11) test_result = FALSE;
    _test_printTestResult(test_result);
}

void test_createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue() {
    _test_printTestNameDescriptionAndResultStub("createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue", "test creates all default values");
    NeuralValue** testColumns = _createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue(5, 15, 7);
    int test_result = TRUE;
    for(int columnIndex = 0; columnIndex < 5; columnIndex++) {
        for(int valueIndex = 0; valueIndex < 15; valueIndex++) {
            if(testColumns[columnIndex][valueIndex] != 7) {
                test_result = FALSE;
            }
        }
    }
    _test_printTestResult(test_result);
}

void test_createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue() {
    _test_printTestNameDescriptionAndResultStub("createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue", "test creates all default values");
    NeuralValue* testColumn = _createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue(15, 7);
    int test_result = TRUE;
    for(int columnIndex = 0; columnIndex < 15; columnIndex++) {
        if(testColumn[columnIndex] != 7) {
            test_result = FALSE;
        }
    }
    _test_printTestResult(test_result);
}

//-----------------------------------------------------------------------------------------
// MODULE TEST FUNCTION
//-----------------------------------------------------------------------------------------
void test_module_neural_network() {
  _test_printModuleTestingMessage();
  test_createColumnOfNueralValuesWithNumberOfEntriesAndDefaultValue();
  test_createColumnsOfNeuralValuesWithNumberOfColumnsNumberOfValuesPerColumnAndDefaultNeuralValue();
  test_mapInputVectorIndexAndOutputVectorIndexForInputVectorOfSizeToWeightVectorIndex();
  test_getContributionOfInputVectorValueWithIndexAndSizeThroughCurrentLayerToOutputValueWithIndex();
}
