/*
 * gcc -g -Wall -fopenmp -o omp_fibo omp_fibo.c 
 * ./omp_fibo (threads) (number of Fibonacci numbers)
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


void Usage(char prog_name[]);

int main(int argc, char* argv[]) {


	//Inicia as funções de controle de tempo
    clock_t start_t, end_t;
    //Inicia a contagem do tempo
    start_t = clock();
   int thread_count, n, i;
   long long* fibo;

   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);

   fibo = malloc(n*sizeof(long long));
   fibo[0] = fibo[1] = 1;
#  pragma omp parallel for num_threads(thread_count)
   for (i = 2; i < n; i++)
      fibo[i] = fibo[i-1] + fibo[i-2];

  	end_t = clock();
    double total;
    total = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Tempo total: %lf\n", total  );


   //printf("The first n Fibonacci numbers:\n");
   //for (i = 0; i < n; i++)
     // printf("%lld ", fibo[i]);
   //printf("\n");
   free(fibo);
   return 0;
}  /* main */

void Usage(char prog_name[]) {
   fprintf(stderr, "usage:  %s <thread count> <number of Fibonacci numbers>\n",
         prog_name);
   exit(0);
}  /* Usage */
