## Specs:

    Arquitetura:           x86_64
    Modo(s) operacional da CPU:32-bit, 64-bit
    Ordem dos bytes:       Little Endian
    CPU(s):                4
    Lista de CPU(s) on-line:0-3
    Thread(s) per núcleo: 2
    Núcleo(s) por soquete:2
    Soquete(s):            1
    Nó(s) de NUMA:        1
    ID de fornecedor:      GenuineIntel
    Família da CPU:       6
    Modelo:                61
    Nome do modelo:        Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz
    Step:                  4
    CPU MHz:               2182.409
    CPU MHz máx.:         2700,0000
    CPU MHz mín.:         500,0000
    BogoMIPS:              4391.85
    Virtualização:       VT-x
    cache de L1d:          32K
    cache de L1i:          32K
    cache de L2:           256K
    cache de L3:           3072K
    CPU(s) de nó0 NUMA:   0-3
    OBS: na verdade são 2 cores e 2 hiperthreads mas o linux não pega



## Análise_final:

   O código sequencial [cracksequencial.c](cracksequencial.c). quando executado resultou em mais de 25 minutos para completar todos os cracks do [crackme.txt](crackme.txt).
//colocar na mesma pasta bin do programa que faz o crack para conseguir executar.

   Já no código [crack.c](crack.c), o programa em parelelo foi executado 3 vezes e ficou entre 17 minutos o tempo médio, usando 4 processos escravos e um mestre , resultando em um speed-up no melhor caso de aproximadente 1.5, o speed-up não é 
tao alto pelo fato de ter vários processos no crackme.txt que executam rapidamente e poucos longos(hipótese:2 hiperthreads 
também prejudicam), se fosse somente longos veriamos claramente uma melhora mais significativa, se compararmos como foi feito o 
particionamento dos processos onde se envia de um em um processo e não em blocos, isso pode oferecer vantagens e desvantagens, 
se estivermos com os cracks que demoram pouco tempo por exemplo e sabe se que todos tem o mesmpo tempo parece mais simples e 
eficaz o particionamento estático, porém quando adicionamos processos longos com essa estratégia empregada ganhamos uma 
flexibilidade onde no caso do estático podem se acabar blocos mais cedo que outros, e se os outros forem longos estamos 
claramente tendo uma perda no desempenho otimizado, o que se resolve no caso dinâmico, pois um processo longo não impede um mais 
curto que vem depois de ser executado ou mesmo um longo. 
   Além disso foi percebido depois que a melhor execução pode na verdade ser com 3 processos escravos pois não precisa ocorrer tantas trocas de contexto como ocorre no com 4 escravos, foi testado e se obteve um desempenho de 15 minutos, speedup de 1,67. 
   Observa-se que testando os dois arquivos teste.txt e rapido.txt o sequencial executa mais rápido devido ao fato de que gasta 
se tempo com criação de processos
e controle dos mesmos.
    

obs:os valores de tempo foram adquiridos com prints iguais no código.


