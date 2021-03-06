//-----------------------------------------------------------------------------------------
// AUTHOR
// ------
// Lumberjacks Incorperated (2018)
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "NeuralNetwork.h"

//-----------------------------------------------------------------------------------------
// DRIVER
//-----------------------------------------------------------------------------------------
int main() {
    printf("Starting the driver...\n");
    NeuralNetwork* aNeuralNetwork = newNeuralNetworkWithSizeOfInputVectorSizeOfNeuralLayerAndNumberOfNeuralLayers(5, 5, 5);
    NeuralValue someInputVector[5] = {12.4, -1231, -2, 123, 12};
    NeuralResultClassification resultClassification;

    resultClassification = classifyResultForInputVectorUnderNeuralNetwork(someInputVector, aNeuralNetwork);
    learnOnInputVectorAndExpectedResultClassification(aNeuralNetwork, someInputVector, TrueResultClassification);
    resultClassification = classifyResultForInputVectorUnderNeuralNetwork(someInputVector, aNeuralNetwork);

    printf("%s\n", resultClassification==TrueResultClassification?"TRUE":"FALSE");
    destroyNeuralNetwork(aNeuralNetwork);
    printf("Finishing the driver...\n");
    return 0;
}
