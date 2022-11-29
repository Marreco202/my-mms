#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include <string.h>
#include <math.h>


#define ERROR_PARAMETERS 1

#define PARAMETERS 5


int char_to_int(char* s){
    int tam = strlen(s);
    int convert = 0;

    for(int i = 0; i<tam; i++){
        convert+= (s[i] - '0') * pow(10,tam -1 -i);
    }
    
    return convert;
}

int main (int args, char* argv[]){

    printf("Executando o simulador...\n"); 

    if(args != PARAMETERS){ //quantidade invalida de parametros
        raise(ERROR_PARAMETERS);
    }

    /*
    argv[1] --> LRU | NRU
    argv[2] --> arquivo.log
    argv[3] --> tamanho da pagina
    argv[4] --> total de memoria
    
    */

    int tam_pagina = char_to_int(argv[3]);
    int total_mem = char_to_int(argv[4]);
    
    printf("Arquivo de entrada: %s\n",argv[2]);
    printf("Tamanho de memoria fisica: %s MB\n", argv[4]);
    printf("Tamanho das páginas: %s KB\n",argv[3]);
    printf("Algoritmo de substituição: %s\n",argv[1]);

    go_simulator(total_mem,tam_pagina,argv);

    //printf("Fim de simulação\n");
    return 0;
}