
typedef struct page Page;
typedef struct pageframe PageFrame;

/*
Aluno: João Victor Godinho Woitschach
Matrícula: 2011401
*/
int get_tam(char* filename);
Page* create_page_table(int tam_pagina);
PageFrame* create_pf(int tam_paginas, int total_mem_fisica);
void go_simulator(int memoria_total,int tam_pagina, char * argv[]);
void raise(int id);
void teste(void);