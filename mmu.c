#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NO_FILE 2


struct page{
    // se pa nao precisa disso || long br, bm; //area de memoria que a pagina esta autorizada a realizar operacoes em
    int frame_index; //indice do quadro na "memoria fisica" || AKA Conteudo bruto da pagina
    
} typedef Page;

struct virtualmem{

    int add;
    char mode;

}typedef VirtualMem;

struct physmem{

    int add;
    //inserir flags

}typedef PhysMem;   


PhysMem* create_phys_mem(int tam_paginas, int total_mem_fisica){

    int tam = (total_mem_fisica * 1000) / tam_paginas;

    PhysMem* memoria = (PhysMem*) malloc(sizeof(PhysMem) * tam);

    for(int i = 0; i<tam; i++){
        memoria[i].add = 0;
    }

    return memoria;
}

void raise(int id){ //raise em erros durante o programa

    switch (id)
    {
    case 1:
        printf("ERRO: QUANTIDADE DE PARAMETROS INVALIDOS!\n");
        break;
    
    case 2:
        printf("ERRO: ARQUIVO NAO ENCONTRADO\n");
        break;

    default:
        printf("ERRO NAO ESPECIFICADO\n");
        break;
    }

    exit(1);
}


Page* create_page_table(int tam_pagina){

    int qtd_pags = pow(2, 32- (int)(ceil(log2(tam_pagina * 1000)))); //calculo para pegar a quantidade de paginas  necessarias

    Page* page_table = (Page*) malloc(sizeof(Page) * qtd_pags);
    
    for(int i = 0; i<qtd_pags; i++){
        page_table[i].frame_index = -1;
        
    }

    return page_table;
}

VirtualMem* create_virtual_mem(int tam){
    VirtualMem* virtual_mem = (VirtualMem*) malloc(sizeof(VirtualMem) * tam);

    for(int i = 0; i<tam; i++){
        virtual_mem->add = 0;
        virtual_mem->mode = 0;
    }

    return virtual_mem;
}

int get_tam(char* filename){

    int i = 0;

    int tam_pagina;
    int tam_mem_fisica;

    printf("Resgatando tamanho de hexs...\n");

    unsigned int qg1;
    char qg2;

    FILE* f = fopen(filename,"r");
    
    if (f == NULL) raise(NO_FILE);

    while(!feof(f)){
        fscanf(f,"%x %c\n", &qg1,&qg2);
        //printf("%d\n",i);
        i++;
    }

    printf("Quantidade de elementos na memoria virtual: %d\n",i);

    fclose(f);

    return i;
}


void insert_values(VirtualMem* virtual_mem,char* filename){
    FILE* f = fopen(filename,"r");

    unsigned int add; //endereco 
    char mode; //modo leitura ou escrita
    int i = 0; //indice para 

    while(!feof(f)){
        fscanf(f,"%x %c\n",&add, &mode);
        
        virtual_mem[i].add = add;
        virtual_mem[i].mode = mode;
        //printf("virtual_mem[%d].add = %lx | virtual_mem[%d].mode = %c\n\n",i,virtual_mem[i].add,i,virtual_mem[i].mode);
        i++;
    }

    fclose(f);
}

void go_simulator(PhysMem* mf,VirtualMem* vm,int tam, int tam_memoria_total, Page* pt,int tam_pagina, char* criteria){ //vm  == virtua_memory array ; pt == page_table array ; criteria == NRU | LRU

    // mf == memoria fisica ; vm == virtual memory ; pt == page table

    int pt_index;
    int page_fault = 0; //contador de page faults
    int next_insert = 0; //proxima posicao vazia da tabela de frames

    int qtd_frames = (tam_memoria_total *1000) / tam_pagina; //quantos frames eu tenho


    for(int i = 0; i<tam; i++){
        pt_index = vm[i].add >> (int)(ceil(log2(tam_pagina*1000))); //indice para a page table, que é onde estaria localizado a memoria fisica

        if(pt[pt_index].frame_index == -1){ //nao foi alocado nada 
            page_fault++;
            
            if(next_insert < tam_pagina){// se houverem indices vazios..,
                
                    
                pt[pt_index].frame_index = next_insert; //fala que o indice da tabela de paginas é o índice para 
                
            }
        }

    }

}