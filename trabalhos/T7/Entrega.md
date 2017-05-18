[ProgramaÃ§Ã£o Paralela](https://github.com/jgmachado90/elc139-2017a) > Trabalhos

# T7: Algoritmo de Dijkstra com open MPI
Disciplina: ProgramaÃ§Ã£o Paralela <br>
Dupla: OtÃ¡vio Oliveira Deon e JoÃ£o Machado<br>
Computador utilizado:  Os testes locais foram realizados utilizando um processador Intel Core i5-4200U, possuindo 2 nÃºcleos, 4 threads, e frequÃªncia de 1.6GHz ou 2.6GHz.

# Resultados

...                 | 1 | 2 | 4 | 8
---		    |---|---|---|---
**1000**|	28.16	|    16.49	|	12.59      |    15.65
**500**| 3.07		|    2.42	|	1.49       |	1.51
**400**| 118.69		|    65.80	|	47.19      |	47.84
**200**| 0.51		|   0.29	|	0.25       | 	0.32

<br><br>
Speedup das execuÃ§Ãµes utilizando tamanho 1000:<br>
1 processo: 28.16 segundos. Com 2 processos, speedup de 1.70. Com 4 processos, speedup de 2.23. Com 8 processos, speedup de 1.79. <br>
Speedup das execuÃ§Ãµes utilizando tamanho 500:<br>
1 processo: 3.07 segundos. Com 2 processos, speedup de 1.26. Com 4 processos, speedup de 2.06. Com 8 processos, speedup de 2.03. <br>
Speedup das execuÃ§Ãµes utilizando tamanho 400:<br>
1 processo: 118.69 segundos. Com 2 processos, speedup de 1.80. Com 4 processos, speedup de 2.51. Com 8 processos, speedup de 2.48.<br>
Speedup das execuÃ§Ãµes utilizando tamanho 200:<br>
1 processo: 0.51 segundos. Com 2 processos, speedup de 1.75. Com 4 processos, speedup de 2.04. Com 8 processos, speedup de 1.59.<br>

<br><br>
# ApresentaÃ§Ã£o
Link: https://docs.google.com/presentation/d/19FnRGDPEeYmrrxtn4-xt1R0YuC3l37fas6f2hdHMTB0/pub?start=false&loop=false&delayms=3000&slide=id.p

<br><br>
# ReferÃªncias
https://computing.llnl.gov/tutorials/mpi/ <br>
http://mpitutorial.com/tutorials/ 
