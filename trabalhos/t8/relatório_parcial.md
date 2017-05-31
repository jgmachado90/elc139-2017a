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



## Análise:

O código sequencial [cracksequencial.c](cracksequencial.c). quando executado resultou em mais de 25 minutos para completar todos os cracks do [crackme.txt](crackme.txt).
,na avaliação final tentará se executar várias vezes, levando em conta que o tempo demora bastante mesmo.
//colocar na mesma pasta bin do programa que faz o crack para conseguir executar.

Já no código [crack.c](crack.c)., foi lido do arquivo também a matriz de caracteres porém a comunicação só ocorre com relação
ao número de processos criados, e mostra-se a origem de cada mensagem trocada como pedido, nota-se que para versão
final do trabalho é necessário ter um controle para passar todos os cracks para um respectivo processo(será passado um crack para um processo,mas tem que passar todos os cracks).

