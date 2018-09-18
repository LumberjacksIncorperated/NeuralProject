//-----------------------------------------------------------------------------------------
// AUTHOR
// ------
// Lumberjacks Incorperated (2018)
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "test_library.h"

//-----------------------------------------------------------------------------------------
// INTERNAL DEFINITIONS
//-----------------------------------------------------------------------------------------
#define TRUE (0==0)
#define FALSE (!TRUE)

//-----------------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-----------------------------------------------------------------------------------------
struct _TEST_RUN {
    TEST_FUNCTION testToRun;
    char* testName;
    char* testDescription;
};

//-----------------------------------------------------------------------------------------
// INTERNAL FUNCTION FORWARD DEFINITIONS
//-----------------------------------------------------------------------------------------
void _printTestNameDescriptionAndResultStub(char* testName, char* testDescription);
void _printTestResult(int testResult);
char* _str_cpy(char* stringToCopy);
void _destroyTest(TEST_RUN testToDestroy);

//-----------------------------------------------------------------------------------------
// INTERNAL FUNCTIONS
//-----------------------------------------------------------------------------------------
void _printTestNameDescriptionAndResultStub(char* testName, char* testDescription) {
    printf("\t<Test>: %s\n", testName);
    printf("\t<Decription>: %s\n", testDescription);
    printf("\t<Result>: ");
}

void _printTestResult(int testResult) {
    if(testResult == TRUE) {
          printf("PASSED\n\n");
    } else {
          printf("FAILED\n\n");
    }
}

char* _str_cpy(char* stringToCopy) {
    char* stringToReturn = malloc(120);
    strcpy(stringToReturn, stringToCopy);
    return stringToReturn;
}

void _destroyTest(TEST_RUN testToDestroy) {
    free(testToDestroy->testName);
    free(testToDestroy->testDescription);
    free(testToDestroy);
}

//-----------------------------------------------------------------------------------------
// EXTERNAL FUNCTIONS
//-----------------------------------------------------------------------------------------
TEST_RUN CREATE_TEST(TEST_FUNCTION testFunction, char* testDescription, char* testName) {
   TEST_RUN newTestRun = malloc(sizeof(struct _TEST_RUN));
   newTestRun->testToRun = testFunction;
   newTestRun->testName = _str_cpy(testName);
   newTestRun->testDescription = _str_cpy(testDescription);
   return newTestRun;
}

void RUN_TEST(TEST_RUN testToRun) {
    _printTestNameDescriptionAndResultStub(testToRun->testName, testToRun->testDescription);
    int result = (*(testToRun->testToRun))();
    _printTestResult(result);
    _destroyTest(testToRun);
}