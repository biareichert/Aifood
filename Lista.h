#ifndef LISTA_H
#define LISTA_H

typedef struct descritorLDDE LDDE;
typedef struct descritorLDDE LDDE;


//Fila pedidos
int criaLDDE(LDDE **pp, unsigned int tamInfo);
int enfileira(LDDE *p, void *novo);
int desenfileira(LDDE *p);
int buscaPrimeiro(LDDE *p, void *reg);// busca sempre o primeiro
int buscarPosLog(LDDE *p, void *reg, unsigned int posLog);
int buscaFim(LDDE *p, void *reg);
int tamanho (LDDE *p);
int destroi(LDDE **p);


int buscarPosLog (LDDE *p, void *reg, unsigned int posLog);
int removerPosLogLDDE (LDDE *p, unsigned int posLog);
int inserirPosLogLDDE(LDDE *p, void *reg, unsigned int posLog);
int removerFimLDDE (LDDE *p);
int inserirFimLDDE (LDDE *p, void *reg);

#endif