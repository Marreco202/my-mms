#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NO_FILE 2


struct page{
    // se pa nao precisa disso || long br, bm; //area de memoria que a pagina esta autorizada a realizar operacoes em
    int frame_index; //indice do quadro na "memoria fisica" || AKA Conteudo bruto da pagina
    
} typedef Page;

struct pageframe{

    int page_id; //sei la 
    int foi_modificada;// foi recentemente modificada?
    
    //inserir flags

}typedef PageFrame;   


PageFrame* create_pf(int tam_paginas, int total_mem_fisica){

    int tam = (total_mem_fisica * 1000) / tam_paginas;

    PageFrame* memoria = (PageFrame*) malloc(sizeof(PageFrame) * tam);

    for(int i = 0; i<tam; i++){
        memoria[i].page_id = -1;
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


void go_simulator(int memoria_total,int tam_pagina, char * argv[]){ //vm  == virtua_memory array ; pt == page_table array ; criteria == NRU | LRU

    int pt_index; //indice da tabela de paginas daquele hex
    int page_fault = 0; //contador de page faults
    int next_insert = 0; //proxima posicao vazia da tabela de frames

    //int qtd_frames = (memoria_total *1000) / tam_pagina; //quantos frames eu tenho

    PageFrame* pf = create_pf(tam_pagina,memoria_total);
    Page *pt = create_page_table(tam_pagina);
    FILE *f = fopen(argv[2],"r"); //abrindo o arquivo
    
    long add; //endereco lido no .log
    char action; //acao a ser feita com o endereco a ser lido

    while(!feof(f)){
        fscanf(f,"%x %c\n", &add, &action);

        pt_index = add >> (int)(ceil(log2(tam_pagina*1000)));

        if(pt[pt_index].frame_index == -1){ //aquele elemento da tabela de paginas nao esteja associada a nenhum pageframe
            page_fault++;

            //procurar espaço vazio    
                //setar o pf[i].page_id = add
                //setar a pt[pt_index] = i
                

          
        } 


        /*
            1 - checar se esse endereco ja foi carregado na memoria
                -- caso nao tenha sido:
                    __ page_fault++
                    __ procurar um espaco vazio, e inserir o hex ali 
                    -- caso nao encontre:
                        __ utilizar algoritmo de substituicao de pagina para trocar as paginas
                    __ setar as flags para o default
            
            2 - atualizar as flags do frame
        */
    }


    fclose(f);
}

    /*

    for(int i = 0; i<tam; i++){
        pt_index = vm[i].add >> (int)(ceil(log2(tam_pagina*1000))); //indice para a page table, que é onde estaria localizado a memoria fisica

        if(pt[pt_index].frame_index == -1){ //nao foi alocado nada 
            page_fault++;
            
            if(next_insert < tam_pagina){// se houverem indices vazios..,
                
                    
                pt[pt_index].frame_index = next_insert; //fala que o indice da tabela de paginas é o índice para 
                
            }
        }

    }
    
    
    */
