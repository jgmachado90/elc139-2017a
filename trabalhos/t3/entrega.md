[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2017a) > Trabalhos

# T3: Programação Paralela Multithread 


Programação Paralela - ELC139-2017a
Professora: Andrea Schwertner Charão

João Gonçalves Machado


## Questões Pthreads


1. Explique como se encontram implementadas as 4 etapas de projeto: particionamento, comunicação, aglomeração, mapeamento (use trechos de código para ilustrar a explicação).

**Particionamento:**

	int wsize = dotdata.wsize;
	int start = offset*wsize;
	int end = start + wsize;

Divide a tarefa entre as threads. Cada uma recebendo uma parte do vetor para calcular em paralelo.

**Comunicação:**

	pthread_mutex_lock (&mutexsum);
	dotdata.c += mysum;
	pthread_mutex_unlock (&mutexsum);

Com uma execução multithread, o mutex garante exclusão mutua para que somente uma thread possa modificar o dotdata.c por vez.

**Aglomeração:**

	dotdata.c += mysum;

dotdata.c recebe as somas das threads, onde cada thread incrementa em sua própria dotdata.c.

**Mapeamento:**

	for (i = 0; i < nthreads; i++) {
		pthread_create(&threads[i], &attr, dotprod_worker, (void *) i);
	}
	pthread_attr_destroy(&attr);
	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}

Criação (pthread_create) e junção (pthread_join) das threads.



2. Considerando o tempo (em segundos) mostrado na saída do programa, qual foi a aceleração com o uso de threads?

	No primeiro programa, com uma única thread, o tempo do programa foi de 
	16,68 segundos.

No segundo programa, com duas threads, o tempo do programa foi de 8,63 segundos.

	Aceleração de 1,93.



3. A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.

		1 thread, para vetor de 1000000 de elementos, com 2000 repetições:

		16679692 usec

		2 threads

		8627080 usec

		4 threads
 
		8593274 usec

		8 threads

		8399543 usec

		16 threads

		8376153 usec




4. Elabore um gráfico/tabela de aceleração a partir dos dados obtidos no exercício anterior.



5. Explique as diferenças entre [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) e [pthreads_dotprod2.c](pthreads_dotprod/pthreads_dotprod2.c). Com as linhas removidas, o programa está correto? 

		O programa pthreads_dotprod2.c não está correto, pois deixa em aberto uma condição de corrida entre as threads, onde elas podem usar de uma mesma variável ao mesmo tempo. O correto seria a utilização de mutex na região crítica, garantindo exclusão mútua.
    


## Preparação OpenMP


1. Nesta parte você vai trabalhar com um programa muito simples que usa OpenMP: [ompsimple.c](openmp/ompsimple.c).

2. Compile o programa usando a flag `-fopenmp`, que habilita OpenMP:

   ```
   gcc -fopenmp -o ompsimple ompsimple.c
   ```
   
3. Execute o programa com 8 threads:

   ```
   OMP_NUM_THREADS=8 ./ompsimple
   ```

## Implementação OpenMP

1. Implemente um programa equivalente a [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) usando OpenMP. 

2. Avalie o desempenho do programa em OpenMP, usando os mesmos dados/argumentos do programa com threads POSIX. 


## Material de apoio

- [POSIX Threads Programming](http://www.llnl.gov/computing/tutorials/pthreads/)  
  Tutorial do Lawrence Livermore National Laboratory (LLNL) sobre Pthreads.

- [Tutorial OpenMP](https://computing.llnl.gov/tutorials/openMP/)  
  Tutorial do Lawrence Livermore National Laboratory (LLNL) sobre OpenMP. 

