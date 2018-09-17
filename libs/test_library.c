#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE (0==0)
#define FALSE (!TRUE)

void _printModuleTestingMessage(char* moduleName);
void _printTestNameDescriptionAndResultStub(char* testName, char* testDescription);
void _printTestResult(int testResult);

typedef int (*TEST_FUNCTION) (void);

struct _TEST_RUN {
    TEST_FUNCTION testToRun;
    char* testName;
    char* testDescription;
    char* moduleName;
};

typedef struct _TEST_RUN *TEST_RUN;

TEST_RUN _createTestWithNameDescriptionStringAndModule(TEST_FUNCTION testFunction, char* testDescription, char* testName, char* moduleName);

#define _test_create(TEST_NAME, TEST_DESCRIPTION, TEST_STRING, MODULE_NAME) \
    (_createTestWithNameDescriptionStringAndModule(TEST_NAME, TEST_DESCRIPTION, TEST_STRING, MODULE_NAME))
//#define _test_run(TEST_RUN) \
//    ()

int test_add(void) {
  int result = (1+1==2);
  return result;
}

int main() {
    printf("Testing\n");
    TEST_FUNCTION first = test_add;
    int result = (*first)();
    TEST_RUN newTestRun = _test_create(test_add, "add", "test", "mod");
    (*(newTestRun->testToRun))();
    (*(newTestRun->testToRun))();
    (*(newTestRun->testToRun))();
    (*(newTestRun->testToRun))();
    return 0;
}

void _printModuleTestingMessage(char* moduleName) {
    printf("\n----------------------------------------------------------------------------------------------\n");
    printf("\t MODULE TESTING: %s\n", moduleName);
    printf("----------------------------------------------------------------------------------------------\n");
}

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
  strcpy(stringToCopy, stringToReturn);
  return stringToReturn;
}

TEST_RUN _createTestWithNameDescriptionStringAndModule(TEST_FUNCTION testFunction, char* testDescription, char* testName, char* moduleName) {
  TEST_RUN newTestRun = malloc(sizeof(struct _TEST_RUN));
  newTestRun->testToRun = testFunction;
  newTestRun->testName = _str_cpy(testName);
  newTestRun->testDescription = _str_cpy(testDescription);
  newTestRun->moduleName = _str_cpy(moduleName);
  return newTestRun;
}
