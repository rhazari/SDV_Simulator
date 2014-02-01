/***********************************************************************************
*  Modulule : variates.h
* 
*  Function:  header for variates modeul
*
*
*  Revisions:
*
**********************************************************************************/
#include <assert.h>             // Needed for assert() macro
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for pow()


int      zipfRN(double alpha, int n);  // Returns a Zipf random variable
double   rand_val(int seed);           // Jain's RNG
double   exponentialRN(double meanRV);         // Exponential RN


#define  FALSE          0       // Boolean false
#define  TRUE           1       // Boolean true

#define EXPONENTIAL_RV 1
#define  ZIPF_RV 2

#define INITIAL_SEED 12345

