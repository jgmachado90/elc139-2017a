

# T4: Bug em OpenMP

## João Gonçalves Machado
Programação Paralela

## Questões

1. No tutorial sobre OpenMP do LLNL, há uma seção de exercícios que mostra códigos com bugs: https://computing.llnl.gov/tutorials/openMP/exercise.html. Qual o bug do programa [omp_bug2.c](https://computing.llnl.gov/tutorials/openMP/samples/C/omp_bug2.c)?

A variavel total estava, por default, setada como shared. Assim, todas as threads compartilhavam a mesma variavel, provocando inconsistencia dos dados. Uma vez que, se a variavel total fosse zerada para uma thread, ela era zerada para todas as outras.



2. Qual seria uma solução para o bug? Explique e forneça o código corrigido.


Para solucionar este bug, setei a variavel total como private.

		  #pragma omp parallel private(total,tid)

Fazendo com que cada thread modifique a sua variavel total, e não interfira na execução das outras.

-Código na pasta do T4.


