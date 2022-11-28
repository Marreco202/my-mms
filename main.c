#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include <string.h>
#include <math.h>
/*
jaja eu me desmuto 

*/

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

    printf("=== SIMULATION START! === \n\n"); 

    if(args != PARAMETERS){ //quantidade invalida de parametros
        raise(ERROR_PARAMETERS);
        exit(1);
    }

    /*
    argv[1] --> LRU | NRU
    argv[2] --> arquivo.log
    argv[3] --> tamanho da pagina
    argv[4] --> total de memoria
    
    */

    int tam_pagina = char_to_int(argv[3]);
    int total_mem = char_to_int(argv[4]);
    

    int tam = get_tam(argv[2]); //pega o tamanho de hexes do arquivo


    VirtualMem* memoria_virtual = create_virtual_mem(tam);
    insert_values(memoria_virtual,argv[2]);
    Page* page_table = create_page_table(tam_pagina);

    printf("tamanho da pagina: %d\n total de memoria: %d\n",tam_pagina,total_mem);


/*
    for(int i = 0; i<args; i++){
        printf("%s\n",argv[i]);
    }
*/

    printf("=== SIMULATION END! === \n");
    return 0;
}