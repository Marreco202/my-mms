#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define NO_FILE 2
#define NO_ALGO 3


struct page{
    // se pa nao precisa disso || long br, bm; //area de memoria que a pagina esta autorizada a realizar operacoes em
    int frame_index; //indice do quadro na "memoria fisica" || AKA Conteudo bruto da pagina
    
} typedef Page;

struct pageframe{

    int page_id; //sei la 
    int foi_modificada;// foi recentemente modificada?
    int foi_referenciada;
    int time; //em que momento de tempo essa pagina foi inserida neste pageframe
    
    //inserir flags

}typedef PageFrame;   


PageFrame* create_pf(int tam_paginas, int total_mem_fisica){

    int tam = (total_mem_fisica * 1000) / tam_paginas;

    PageFrame* memoria = (PageFrame*) malloc(sizeof(PageFrame) * tam);

    for(int i = 0; i<tam; i++){
        memoria[i].page_id = -1;
        memoria[i].foi_modificada = 0;
        memoria[i].foi_referenciada = 0;
        memoria[i].time = 0;
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
    
    case 3:
        printf("ERRO: ALGORITMO DE SUBSTITUICAO NAO EXISTENTE");
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

int find_next_insert(PageFrame *pf, int tam){

    for(int i =0; i< tam; i++){
        if(pf[i].page_id == -1) return i;
    }
    return -1; //nao foi encontrado nenhum espaco vazio na memoria fisica
}

int lru(PageFrame *pf, int tam_pf){

    int tempo_ma = -1; //tempo mais antigo = numero da iteracao que aquela pagina foi inserida 
    int target_index;

    for(int i = 0; i<tam_pf; i++){
        if(tempo_ma != -1){
            if(tempo_ma > pf[i].time){
              tempo_ma = pf[i].time;
              target_index = i;  
            } 
        }
        else tempo_ma = pf[i].time;
    }
    return target_index;
}


int swap(PageFrame *pf, Page *pt, int old_pt_index, int new_pt_index){ //troca de pagina

    int qg = pt[old_pt_index].frame_index; //indice da tabela de paginas que ira ser trocado para
    pt[old_pt_index].frame_index = -1;
    pt[new_pt_index].frame_index = qg;

    return qg; //variavel que retorna o indice de page frame que iremos trabalhar em cima
}

int troca_de_paginas(PageFrame *pf, Page* pt, int tam_pf, char* criterio, int to_insert_index){ //to_insert e aquele que queremos inserir em PageFrame

    int target_index = -1;

    if(strcmp(criterio,"NRU")){ 
        printf("hello there!");
    }
    else if(strcmp(criterio,"LRU")){
        target_index = lru(pf,tam_pf);
    }
    else raise(NO_ALGO);

    return swap(pf,pt,target_index,to_insert_index); //to insert index --> indice que vamos inserir na pf ; target --> indice do elemento a ser trocado
}



void go_simulator(int memoria_total,int tam_pagina, char * argv[]){ //vm  == virtua_memory array ; pt == page_table array ; criteria == NRU | LRU

    int pt_index; //indice da tabela de paginas daquele hex
    int next_insert = 0; //proxima posicao vazia da tabela de frames
    
    int page_fault = 0; //contador de page faults
    int time = 0; //contador de tempo
    int paginas_escritas = 0;

    int tam_pf = (memoria_total * 1000) / tam_pagina;

    //int qtd_frames = (memoria_total *1000) / tam_pagina; //quantos frames eu tenho

    PageFrame* pf = create_pf(tam_pagina,memoria_total);
    Page *pt = create_page_table(tam_pagina);
    FILE *f = fopen(argv[2],"r"); //abrindo o arquivo
    char* criterio; //criterio para substituição de página
    
    int add; //endereco lido no .log
    char action; //acao a ser feita com o endereco a ser lido

    if(f == NULL) raise(NO_FILE);

    while(!feof(f)){
        fscanf(f,"%x %c\n", &add, &action);

        pt_index = add >> (int)(ceil(log2(tam_pagina*1000)));

        if(pt[pt_index].frame_index == -1){ //aquele elemento da tabela de paginas nao esteja associada a nenhum pageframe
            page_fault++;
            next_insert = find_next_insert(pf,tam_pf); //procura espaco vazio

            if(next_insert != -1){ //encontrei um espaco vazio
                pt[pt_index].frame_index = next_insert;
                pf[next_insert].page_id = add;
                pf[next_insert].time = time;
                pf[next_insert].foi_referenciada = 1;

                if(action == 'W'){
                    pf[next_insert].foi_modificada = 1;
                    paginas_escritas++;
                }


            }
            else{// nao encontrei um espaco vazio
                int qg = troca_de_paginas(pf,pt,tam_pf,criterio,pt_index);
                pf[qg].page_id = add;
                pf[qg].time = time;

                if(action == 'W'){
                    pf[qg].foi_modificada = 1;
                    paginas_escritas++;
                }
            }
  
        } 
    }



    time++;
    fclose(f);

    printf("Numero de Faltas de Páginas: %d\n" ,page_fault);
    printf("Numero de Páginas Escritas: %d\n",paginas_escritas);

    /*
            1 - checar se esse endereco ja foi carregado na memoria
                -- caso nao tenha sido:
                    __ page_fault++
                    __ procurar um espaco vazio, e inserir o hex ali 
                    -- caso nao encontre:
                        __ utilizar algoritmo de substituicao de pagina para trocar as paginas
                    __ setar as flags para o default
                    __tempo++
            
            2 - atualizar as flags do frame
        */
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
