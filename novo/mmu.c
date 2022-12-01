#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define NO_FILE 2
#define NO_ALGO 3

void teste(void){

    printf("Hello there!\n");
}

struct page{
    int frame_index; //indice do quadro na "memoria fisica"
    
} typedef Page;

struct pageframe{

    int page_id; 
    int foi_modificada;
    int foi_referenciada;
    int time; //em que momento de tempo essa pagina foi inserida neste pageframe

}typedef PageFrame;   


PageFrame* create_pf(int tam_paginas, int total_mem_fisica){ //DONE

    int tam = (total_mem_fisica * 1000) / tam_paginas;
    printf("total de itens na page frame: %d\n",tam);

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


Page* create_page_table(int tam_pagina){ //DONE
 
    int qtd_pags = pow(2, 32- (int)(ceil(log2(tam_pagina * 1000)))); //calculo para pegar a quantidade de paginas  necessarias
    //printf("quantidade de paginas da tabela de paginas: %d\n",qtd_pags);

    Page* page_table = (Page*) malloc(sizeof(Page) * qtd_pags);
    
    for(int i = 0; i<qtd_pags; i++){
        page_table[i].frame_index = -1;
        
    }

    return page_table;
}

int find_next_insert(PageFrame *pf, int tam){ //DONE

    for(int i =0; i< tam; i++){
        if(pf[i].page_id == -1) return i;
    }
    return -1; //nao foi encontrado nenhum espaco vazio na memoria fisica
}

int nru(PageFrame *pf, int tam_pf){

    int tempo_ma = -1; //tempo mais antigo = numero da iteracao que aquela pagina foi inserida 
    int target_index;

    for(int i = 0; i<tam_pf; i++){ //percorre page frame indice a indice procurando o menor tempo
        //printf("i: %d\n",i);
        if(tempo_ma != -1){
            if(pf[i].time < tempo_ma){ //tempo da interação anterior
              tempo_ma = pf[i].time;
              target_index = i;  
              //printf("novo menor tempo! %d\n",pf[i].time);
            } 
        }
        else{//primeiro menor tempo!
          //printf("tempo_ma: %d\n",tempo_ma);  
          tempo_ma = pf[i].time;
          target_index = i;
          
        } 
        //printf("pf[%d].time: %d\n",i,pf[i].time);
        //sleep(1);
    }
    return target_index;
}


int swap(PageFrame *pf, Page *pt, int old_pt_index, int new_pt_index){ //troca de pagina

    int qg = pt[old_pt_index].frame_index; //indice da tabela de paginas que ira ser trocado para
    pt[old_pt_index].frame_index = -1;
    pt[new_pt_index].frame_index = qg;

    return qg; //variavel que retorna o indice de page frame que iremos trabalhar em cima
}

int lru(PageFrame *pt, int tam_pf){

    int menos_referenciada = -1;
    int target_index;


    for(int i = 0; i<tam_pf;i++){
        if(menos_referenciada != -1){
            if(pt[i].foi_referenciada < menos_referenciada){
                menos_referenciada = pt[i].foi_referenciada;
                target_index = i;
            }

        }else{
            menos_referenciada = pt[i].foi_referenciada;
            target_index = i;
        }
    }

    return target_index;
}

int troca_de_paginas(PageFrame *pf, Page* pt, int tam_pf, char* criterio, int to_insert_index){ //to_insert e aquele que queremos inserir em PageFrame

    int target_index = -1;

    int comp1,comp2;

    comp1 = strcmp(criterio,"LRU");
    comp2 = strcmp(criterio,"NRU");

    //printf("criterio : %s\n",criterio);
    //printf("valor de strcmp com LRU : %d\n",strcmp(criterio,"LRU"));
    //printf("valor de strcmp com NRU : %d\n",strcmp(criterio,"NRU"));


    if(comp1 == 0){ 
        target_index = lru(pf,tam_pf);
    }
    else if(comp2 == 0){
        target_index = nru(pf,tam_pf);
    }
    else raise(NO_ALGO);

    return swap(pf,pt,target_index,to_insert_index); //to insert index --> indice que vamos inserir na pf ; target --> indice do elemento a ser trocado
}



void go_simulator(int memoria_total,int tam_pagina, char * argv[]){ //vm  == virtual_memory array ; pt == page_table array ; criteria == NRU | LRU

    unsigned int pt_index; //indice da tabela de paginas daquele hex
    unsigned int next_insert = 0; //proxima posicao vazia da tabela de frames
    
    int page_fault = 0; //contador de page faults
    int time = 0; //contador de tempo
    int paginas_escritas = 0;

    int tam_pf = (memoria_total * 1000) / tam_pagina;
    //printf("tamanho total da page frame: %d\n",tam_pf);

    //int qtd_frames = (memoria_total *1000) / tam_pagina; //quantos frames eu tenho

    PageFrame* pf = create_pf(tam_pagina,memoria_total);
    Page *pt = create_page_table(tam_pagina);
    FILE *f = fopen(argv[2],"r"); //abrindo o arquivo
    
    unsigned int add; //endereco lido no .log
    char action; //acao a ser feita com o endereco a ser lido

    if(f == NULL) raise(NO_FILE);

    while(!feof(f)){
        fscanf(f,"%x %c\n", &add, &action);
    

        pt_index = add >> (int)(ceil(log2(tam_pagina*1000)));
        ;

        if(pt[pt_index].frame_index == -1){ //aquele elemento da tabela de paginas nao esteja associada a nenhum pageframe
            //printf("pagina nao inserida na page frame, procurando espaco vazio...\n");
            page_fault++;
            next_insert = find_next_insert(pf,tam_pf); //procura espaco vazio

            if(next_insert != -1){ //encontrei um espaco vazio
                //printf("encontrei indice vazio na page frame!: %d\n",next_insert);
                pt[pt_index].frame_index = next_insert;
                pf[next_insert].page_id = add;
                pf[next_insert].time = time;
                

                if(action == 'W'){
                    pf[next_insert].foi_modificada = 1;
                    paginas_escritas++;
                }


            }
            else{// nao encontrei um espaco vazio
                //printf("nao encontrei enspaço vazio, efetuando troca de pagina");
                int qg = troca_de_paginas(pf,pt,tam_pf,argv[1],pt_index);
                pf[qg].page_id = add;
                pf[qg].time = time;

                if(action == 'W'){
                    pf[qg].foi_modificada = 1;
                    paginas_escritas++;
                }
            }
  
        }else{ //aquele elemento esta relacionado a algum page frame
            pf[pt[pt_index].frame_index].foi_referenciada++;
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