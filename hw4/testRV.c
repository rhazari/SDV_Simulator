/***********************************************************************************
*  Module:  testRV
* 
*  Function:   Test driver for random variate routines
*
*  Revisions:
*    Created 11/9/2013
*
**********************************************************************************/
#include "variates.h"

void main(void)
{
  FILE   *fp;                   // File pointer to output file
  char   file_name[256];        // Output file name string
  char   temp_string[256];      // Temporary string variable
  double shape;                 // Alpha parameter
  double n;                     // N parameter
  int    num_values;            // Number of values
  int    i;                     // Loop counter

  double beta=0.0;                 // exponential beta  parameter
  double  sampleRV;               // random variable
  int  sampleRVINT;           // random variable
  int   RV_type=0;                // defined in variates.h


  printf("Output file name ===================================> ");
  scanf("%s", file_name);
  fp = fopen(file_name, "w");
  if (fp == NULL)
  {
    printf("ERROR in creating output file (%s) \n", file_name);
    exit(1);
  }

  printf("What type of Random Variable:  \n");
  printf(" 1)Exponential Random Variable  \n");
  printf(" 2)Zipf Random Variable  \n");
  scanf("%s", temp_string);
  RV_type = atoi(temp_string);
  if ( (RV_type != EXPONENTIAL_RV) && (RV_type != ZIPF_RV))
  {
    printf(" ERROR- bad Random Variable type (%d ??) \n", RV_type);
    exit(1);
  }

 
  // Prompt for random number seed and then use it
  //printf("Random number seed (greater than 0) ================> ");
  //scanf("%s", temp_string);
  //rand_val((int) atoi(temp_string));

  printf("How many samples ?\n");
  scanf("%s", temp_string);
  num_values = atoi(temp_string);

  if (RV_type == EXPONENTIAL_RV) {
    printf("Input the desired mean  value\n");
    scanf("%s", temp_string);
    double tmpP =  atof(temp_string);
    if (tmpP > 0)
      beta  = 1/tmpP;

    printf("-------------------------------------------------------- \n");
    printf("-  Generating %d EXPONENTIAL samples to file              - \n",num_values);
    printf("-------------------------------------------------------- \n");

    // Generate and output zipf random variables
    for (i=0; i<num_values; i++)
    {
      sampleRV = exponentialRN(beta);
      fprintf(fp, "%3.3f \n", sampleRV);
    }

  } else {

 
    printf("Input the desired shape value\n");
    scanf("%s", temp_string);
    shape = atof(temp_string);

    printf("Input the number of items that are ranked \n");
    scanf("%s", temp_string);
    n = atoi(temp_string);


    printf("-------------------------------------------------------- \n");
    printf("-  Generating %d ZIPF samples to file                          - \n",num_values);
    printf("-------------------------------------------------------- \n");

    // Generate and output zipf random variables
    for (i=0; i<num_values; i++)
    {
      sampleRVINT = zipfRN(shape, n);
      fprintf(fp, "%d \n", sampleRVINT);
    }
  }

  // Output "done" message and close the output file
  printf("-------------------------------------------------------- \n");
  printf("-  Done! \n");
  printf("-------------------------------------------------------- \n");
  fclose(fp);
}

