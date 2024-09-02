#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista_priv.h"

int criaLDDE (LDDE **pp, unsigned int tamInfo) {

        // Declaração de variáveis
        LDDE *ptr;

        // Instruções
        ptr = (LDDE*) malloc (1 * sizeof (LDDE));
        if (!ptr) {
            return 0;
        }
        ptr->primeiro = NULL;
        ptr->ultimo = NULL;
        ptr->tamInfo = tamInfo;
        *pp = ptr;
        return 1;
    }

int enfileira (LDDE *p, void *novo) {

        // Declaração de variáveis
        noLDDE *temp;

        // Instruções
        temp = (noLDDE*) malloc (1 * sizeof (noLDDE));
        if (!temp) {
            return 0;
        }
        temp->dados = (void*) malloc (p->tamInfo);
        if (!temp->dados) {
            free (temp);
            return 0;
        }
        memcpy (temp->dados, novo, p->tamInfo);
        if (p->ultimo == NULL) {
            p->ultimo = temp;
            p->primeiro = temp;
            temp->tras = NULL;
            temp->frente = NULL;
        }
        else {
            temp->frente = p->ultimo;
            temp->tras = NULL;
            p->ultimo->tras = temp;
            p->ultimo = temp;
        }
        return 1;
    }

int desenfileira (LDDE *p) {

        // Declaração de variáveis
        noLDDE *aux;

        // Instruções

        if (p->primeiro == NULL) {
            return 0;
        }
        else if (p->primeiro == p->ultimo) {
            free (p->primeiro->dados);
            free (p->primeiro);
            p->primeiro = NULL;
            p->ultimo = NULL;
        }
        else {
            aux = p->primeiro;
            p->primeiro = aux->tras;
            aux->tras->frente = NULL;
            free (aux->dados);
            free (aux);
        }
        return 1;
    }

    int inserirPosLogLDDE (LDDE *p, void *novo, unsigned int posLog) {

        // Declaração de variáveis
        noLDDE *temp;
        noLDDE *aux;
        int i;

        // Instruções

        // Criando o nó que será inserido
        temp = (noLDDE*) malloc (1 * sizeof (noLDDE));
        if (!temp) {
            free (temp);
            return 0;
        }
        temp->dados = (void*) malloc (p->tamInfo);
        if (!temp->dados) {
            free (temp->dados);
            free (temp);
            return 0;
        }
        memcpy (temp->dados, novo, p->tamInfo);

        // Inserindo o nó criado na lista
        if (posLog == 0) {
            enfileira (p, novo);
        }
        else if (posLog == tamanho(p)) {
            inserirFimLDDE (p, novo);
        }
        else {
            aux = p->primeiro;
            for (i=0; i<=posLog-1; i++) {
                aux = aux->tras;
            }
            temp->tras = aux;
            temp->frente = aux->frente;
            aux->frente->tras = temp;
            aux->frente = temp;
        }
        return 1;
    }

	int inserirFimLDDE (LDDE *p, void *novo) {

        // Declaração de variáveis
        noLDDE *temp;

        // Instruções
        temp = (noLDDE*) malloc (1 * sizeof (noLDDE));
        if (!temp) {
            free (temp);
            return 0;
        }
        temp->dados = (void*) malloc (p->tamInfo);
        if (!temp->dados) {
            free (temp->dados);
            free (temp);
            return 0;
        }
        memcpy (temp->dados, novo, p->tamInfo);
        if (p->ultimo == NULL) {
            p->ultimo = temp;
            p->primeiro = temp;
            temp->tras = NULL;
            temp->frente = NULL;
        }
        else {
            temp->frente = p->ultimo;
            temp->tras = NULL;
            p->ultimo->tras = temp;
            p->ultimo = temp;
        }
        return 1;
    }


int removerPosLogLDDE (LDDE *p, unsigned int posLog) {

        // Declaração de variáveis
        noLDDE *aux;
        int i;

        // Instruções
        if (posLog == 0) {
            desenfileira (p);
        }
        else if (posLog == tamanho(p)-1) {
            removerFimLDDE (p);
        }
        else {
            aux = p->primeiro;
            for (i=0; i<=posLog-1; i++) {
                aux = aux->tras;
            }
            aux->frente->tras = aux->tras;
            aux->tras->frente = aux->frente;
            free (aux->dados);
            free (aux);
        }
        return 1;
    }

int removerFimLDDE (LDDE *p) {

        // Declaração de variáveis
        noLDDE *aux;

        // Instruções

        if (p->ultimo == NULL) {
            return 0;
        }
        else if (p->primeiro == p->ultimo) {
            free (p->ultimo->dados);
            free (p->ultimo);
            p->primeiro = NULL;
            p->ultimo = NULL;
        }
        else {
            aux = p->ultimo;
            p->ultimo = aux->frente;
            aux->frente->tras = NULL;
            free (aux->dados);
            free (aux);
        }
        return 1;
    }

int buscaPrimeiro(LDDE *p, void *reg) {

        // Instruções
        if (p->primeiro == NULL) {
            return 0;
        }
        else {
            memcpy (reg, p->primeiro->dados, p->tamInfo);
            return 1;
        }
    }


    int tamanho(LDDE *p) {

        // Declação de variáveis
        int tamanho = 0;
        noLDDE *aux;

        // Instruções
        aux = p->primeiro;
        if (aux == NULL) {
            return (tamanho);
        }
        else {
            tamanho++;
            while (aux->tras != NULL) {
                aux = aux->tras;
                tamanho++;
            }
            return (tamanho);
        }
    }


    int buscarPosLog (LDDE *p, void *reg, unsigned int posLog) {

        // Declaração de variáveis
        noLDDE *aux;
        int i;

        // Instruções

        if (posLog == 0) {
            buscaPrimeiro (p, reg);
        }
        else if (posLog == tamanho(p)-1) {
            buscaFim (p, reg);
        }
        else {
            aux = p->primeiro;
            for (i=0; i<=posLog-1; i++) {
                aux = aux->tras;
            }
            memcpy (reg, aux->dados, p->tamInfo);
        }
        return 1;
    }

    int buscaFim (LDDE *p, void *reg) {

        // Instruções
        if (p->ultimo == NULL) {
            return 1;
        }
        else {
            memcpy (reg, p->ultimo->dados, p->tamInfo);
            return 1;
        }
    }

int destroi (LDDE **p) {
        // Declaração de variáveis
        noLDDE *aux;

        // Instruções
        while ((*p)->primeiro != NULL) {
            aux = (*p)->primeiro->tras;
            free ((*p)->primeiro->dados);
            free ((*p)->primeiro);
            (*p)->primeiro = aux;
        }
        free (*p);
        (*p) = NULL;
        return 1;
    }
		