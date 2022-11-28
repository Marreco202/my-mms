
typedef struct page Page;
typedef struct pageframe PageFrame;

int get_tam(char* filename);
Page* create_page_table(int tam_pagina);
PageFrame* create_pf(int tam_paginas, int total_mem_fisica);