#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct page{
    // se pa nao precisa disso || long br, bm; //area de memoria que a pagina esta autorizada a realizar operacoes em
    int frame; //indice do quadro na "memoria fisica" || AKA Conteudo bruto da pagina

} typedef Page;



Page* create_page_table(int tam){

    Page* page_table = (Page*) malloc(sizeof(Page) * tam);
    
    for(int i = 0; i<tam; i++){
        page_table[i].frame = 0;
        
    }

    return page_table;
}