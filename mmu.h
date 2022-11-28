
typedef struct page Page;
typedef struct virtualmem VirtualMem;

int get_tam(char* filename);
Page* create_page_table(int tam_pagina);
VirtualMem* create_virtual_mem(int tam);
void insert_values(VirtualMem* virtual_mem,char* filename);