#ifndef TEST_LIBRARY_H
#define TEST_LIBRARY_H
//-----------------------------------------------------------------------------------------
// AUTHOR
// ------
// Lumberjacks Incorperated (2018)
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//-----------------------------------------------------------------------------------------

/*
   A definition for the the test run unit
*/
typedef struct _TEST_RUN *TEST_RUN;

/*
   A definitition of the runnable test functions that are used by the test units
*/
typedef int (*TEST_FUNCTION) (void);


//-----------------------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------------------

/*
   Create a test unit with a test function to run along with print strings

   testFunction
      - A pointer to the function that contains the test
   testDescription
      - A string description of the test
   testName
      - The name of the test as a string 

   Returns
      - A test unit that can be run by another function from this interface
*/
TEST_RUN CREATE_TEST(TEST_FUNCTION testFunction, char* testDescription, char* testName);

/*
   Run the test and print test results

   testToRun
      - The test unit that must have been previously created
*/
void RUN_TEST(TEST_RUN testToRun);

#endif
