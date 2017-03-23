[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2016a) > Trabalhos

# T2: Experiências com Profilers

obs: Tive alguns problemas para realizar o trabalho, por isso estou entregando fora do pra

## João Gonçalves Machado

## Parte 1


- (a) O perfil é afetado pelas opções de configuração?
  			- Sim, o tamanho das entradas influenciam diretamente no resultado do perfil. Entrando com valores muito
  			baixos(3000 90), os resultados de tempo são praticamente todos 0.0 , ou muito próximos de 0.0 . Em contrapartida,
  			quando entramos com valores altos(30000000  90) os tempos de execução aumentam consideravelmente, o que já era esperado
  			considerando o programa dado.


teste 1 (300  90)

 %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00        2     0.00     0.00  wtime
  0.00      0.00     0.00        1     0.00     0.00  dot_product
  0.00      0.00     0.00        1     0.00     0.00  init_vectors

teste 2 (300000 90)
 %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 89.74      0.21     0.21        1   206.39   206.39  dot_product
  0.00      0.21     0.00        2     0.00     0.00  wtime
  0.00      0.21     0.00        1     0.00     0.00  init_vectors

teste 3 (30000000 90)
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 92.28     22.16    22.16        1    22.16    22.16  dot_product
  1.76     22.59     0.42        1     0.42     0.42  init_vectors
  0.00     22.59     0.00        2     0.00     0.00  wtime



- (b) Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
			-A função dot_product é evidentemente a que gasta o maior tempo de execução, por isso é a 
			melhor candidata para paralelização.

