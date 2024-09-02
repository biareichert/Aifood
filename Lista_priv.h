#ifndef LISTA_PRIV_H
#define LISTA_PRIV_H

#include "Lista.h"    
	
struct NoLDDE{
	  void *dados;
	  struct NoLDDE *frente;
    struct NoLDDE *tras;
};
typedef struct NoLDDE noLDDE;

struct descritorLDDE {
    struct NoLDDE *primeiro;
    struct NoLDDE *ultimo;
    unsigned int tamInfo;
};
typedef struct descritorLDDE LDDE;

#endif