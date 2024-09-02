#include "Lista.h"

typedef struct Pedidos{
int quantidade;
char nome[50];
int nAtendente;
} pPedidos;


typedef struct estoques{
	unsigned int estoqueInicial;
	unsigned int estoqueAtual;
	char nome[50];
	unsigned int preco;
} pEstoque;

/*funções gerais*/
int verificaPedidos(char nomeLanche[], int qntLanches);
unsigned int procuraValor(char nomeLanche[], int quantidadeLanches);
int leArquivoEstoque();
void relatorios();
void *atendente_Thread(void* argp);
void *caixa_Thread(void *argp);
void iniciandoThreads();
