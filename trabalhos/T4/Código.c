#include "stdafx.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int _tmain(int argc, _TCHAR* argv[])
{
int nthreads, i, tid;
float total;

/*** Spawn parallel region ***/
#pragma omp parallel private(total,tid) //correto
  {
  /* Obtain thread number */
  tid = omp_get_thread_num();
  total= 0.0;
  /* Only master thread does this */
  if (tid == 0) {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d is starting...\n",tid);

  #pragma omp parallel for schedule(static,10)\
  private(i)\
  reduction(+:total)
  for (i=0; i<1000000; i++) 
     total = total + i*1.0;

  printf ("Thread %d is done! Total= %e\n",tid,total);

  } /*** End of parallel region ***/
}
