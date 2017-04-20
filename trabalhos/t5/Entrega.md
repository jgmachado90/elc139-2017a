[Programação Paralela](https://github.com/otaviodeon/elc139-2017a) > Trabalhos

# T5: Monte Carlo em OpenMP

Disciplina: Programação Paralela (ELC139)<br/>
Aluno: Otávio Oliveira Deon e João Machado

Os testes foram realizados utilizando um processador Intel Core i5-4200U, possuindo 2 núcleos, 4 threads, e frequência de 1.6GHz ou 2.6GHz.

**1ª solução:** <br>
Na primeira solução, apenas foram paralelizados laços do *Forest.cpp* que varrem a floresta inteira. O diferencial entre os testes dessas paralelizações é o tamanho da floresta, pois é essa variável que acaba influenciando os ganhos/perdas com a paralelização feita. Então, quanto menor o tamanho da floresta, maior é o overhead do trabalho com as threads e menos vale a pena fazer tais paralelizações. O número de threads usadas também influencia o resultado - por exemplo, executando com argumentos pequenos (30, 100, 100), com 2 threads o tempo médio da execução do programa foi de 2.9 segundos. Com 4 threads, o tempo foi de 2.4. A execução sequencial levou cerca de 2.1 segundos.
Executando com o tamanho da floresta dobrado (60) e 4 threads, os resultados da paralelização foram melhores, próximos da execução sequencial ou até mesmo levemente mais rápidos (média de 12.9 segundos). 


