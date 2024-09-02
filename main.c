/*
	Trabalho para a disciplina de SOP
	Alunos: Beatriz Reichert
			Geremias Corrêa
*/

//BACKUP DAQUI
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "utils.h"
#include "cliente.h"


pthread_barrier_t barreira;
pthread_cond_t fimAt = PTHREAD_COND_INITIALIZER;
pthread_mutex_t t2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t t = PTHREAD_MUTEX_INITIALIZER;

//Variaveis globais:
int nthr;
char *nomearq;
unsigned long receitatotal=0;
int contadorFimThreads=0;

LDDE *ptr = NULL; //ponteiro para a lista de  pedidos
LDDE *ptr2 = NULL; //ponteiro para a lista de estoque

//Struct dos nthr atendentes
struct atendentes{
	unsigned int lucroAtendente;
	int pedidosSucesso;
};

struct atendentes *at;

//Função para verificar a validade de um pedido e decrementar o estoque se for válido
int verificaPedidos(char nomeLanche[], int qntLanches){
	  int i;
		int flag=0;
		pEstoque reg2;

		//Comparo se o lanche e a quantidade pedida seria valida
	  for(i=0;i<tamanho(ptr2);i++){
		  	buscarPosLog(ptr2,&reg2, i);
		   	if(strcmp(nomeLanche,reg2.nome)==0){
			   	flag=1;
					if(qntLanches <= reg2.estoqueAtual){
			    		flag=2;
		   			}
					break;
			}
	  }
	  if(flag == 0){//Pedido inexistente em estoque
	    	return 0;
	  }
	  else if (flag==1){
		  	return 0;
		}
		reg2.estoqueAtual -= qntLanches;
		removerPosLogLDDE(ptr2, i); //Removo o lanche atual do estoque
		inserirPosLogLDDE(ptr2, &reg2, i); //Insiro ele atualizado
  	return 1;
}

//Função para retornar o preço de um certo lanche.
unsigned int procuraValor(char nomeLanche[], int quantidadeLanches){
		int i;
		pEstoque reg2;
  	for(i=0;i<tamanho(ptr2);i++){
	  		buscarPosLog(ptr2, &reg2, i);
	   		if(strcmp(nomeLanche,reg2.nome)==0){
		    		return reg2.preco;
	   		}
		}
		return 0;
}

//Função para ler o arquivo de estoque.
int leArquivoEstoque(){
    int i;
		pEstoque reg2;
    FILE *arq;
	  if ((arq = fopen(nomearq, "r")) == NULL) {
		    printf( "ARQUIVO NAO PODE SER ABERTO \n");
		    return 1;
	  }else{
		    i = 0;
		    while(!feof(arq)){
			       fflush(stdin);
			       fscanf(arq,"%s %u %u", reg2.nome, &reg2.preco, &reg2.estoqueInicial);
			       reg2.estoqueAtual = reg2.estoqueInicial;
				   	 enfileira(ptr2,&reg2);
				   	 i++;
		    }
		}
		fclose(arq);
  	return 0;
}

//Função para printar em tela os relatórios.
void relatorios(){
		int i; 	pEstoque reg2;
		printf ("\n\n ********** Relatorio financeiro ********* \n\n");
		printf ("Atendente    Pedidos    Valor(R$)\n");
		//Percorre a struct atendentes e os printa.
		for(i=0;i<nthr;i++){
				receitatotal+=at[i].lucroAtendente;
				printf (fmtfinanc,i+1,at[i].pedidosSucesso,at[i].lucroAtendente);
		}
		printf ("\n********************\n");
		printf ("\n\nReceita total: %lu\n", receitatotal);


		printf ("\n******** Estoque de lanches ******\n\n");
		printf ("Lanche \t\t    Inicial  \tFinal\n");
		//Percorrea a lista de lanche e os printa.
		for(i=0;i<tamanho(ptr2);i++){
				buscarPosLog(ptr2, &reg2, i);
				printf (fmtestoque, reg2.nome, reg2.estoqueInicial, reg2.estoqueAtual);
		}
		printf ("\n********************\n");
}

//Função thread para os atendentes.
void *atendente_Thread(void* argp){

		pPedidos reg;
		long idAt = (long)argp;
	 	int quantidadeLanche, qtdlida;
	  	char nomeLanche[50];
		char *arqPedido;
		char aux[3];
		sprintf (aux,"%ld",idAt+1);

		//Alocação, cópia e concatenação da minha string para o arquivo.
		arqPedido = (char *)malloc (sizeof(char) * (strlen(nomearq) + 1 + strlen(aux)));
		strcpy (arqPedido, nomearq);
		strcat (arqPedido, "-");
		strcat (arqPedido, aux);

		//Barreira que trava o atendente até todos os atendentes e caixa chegarem em tal
		pthread_barrier_wait(&barreira);

		FILE *arqn;
		if ((arqn = fopen(arqPedido, "r")) == NULL) {
				printf( "ARQUIVO NAO PODE SER ABERTO \n");
		}
		else{
				while(!feof(arqn)){
						fflush(stdin);
						qtdlida=fscanf(arqn,"%s %d", nomeLanche, &quantidadeLanche );
						if(qtdlida!=2)
							break;
						//qtdlida quebra a leitura se for diferente de 2 garantir sinalização de fim de arquivo.
						pthread_mutex_lock(&t2);//Início RC pelo acesso à lista estoque
						if(verificaPedidos(nomeLanche, quantidadeLanche) == 0){ //Caso pedido seja invalido
								pthread_mutex_unlock(&t2); //Fim RC acesso à lista estoque
						}
						else{ //Caso pedido seja valido
								pthread_mutex_unlock(&t2); //Fim RC acesso à lista estoque
								memcpy(reg.nome, nomeLanche,sizeof(char) * 50);
								reg.quantidade = quantidadeLanche;
								reg.nAtendente = idAt;
								pthread_mutex_lock(&t); //Lock para garantir exclusão mutua quando enfileirar
								enfileira(ptr,&reg); //Enfileira o pedido para o caixa
								pthread_cond_signal(&fimAt);
								pthread_mutex_unlock(&t); //Unlock da exclusão mutua quando enfileirar.
						}
				}
		}

		pthread_mutex_lock(&t);
		contadorFimThreads++; //Contador para se saber quantos atendentes já finalizaram seus pedidos.
		if(contadorFimThreads==nthr){
				reg.quantidade=0;
				reg.nAtendente = idAt;
				enfileira(ptr,&reg);
				pthread_cond_signal(&fimAt);
		}
		pthread_mutex_unlock(&t);
		fclose(arqn);
		pthread_exit(NULL);
}

//Função para a thread caixa.
void *caixa_Thread(void *argp){
		int atendente;
		pPedidos reg;
		float valor;

		//Barreira que trava o caixa até todos os atendentes e caixa chegarem em tal
		pthread_barrier_wait(&barreira);

		while(tamanho(ptr)!=0 || contadorFimThreads!=nthr){//Condição de vida do caixa
					pthread_mutex_lock(&t);
					while(tamanho (ptr)==0 && contadorFimThreads!=nthr){//Condição de espera do caixa
							pthread_cond_wait(&fimAt,&t);
					}
					pthread_mutex_unlock(&t);
					buscaPrimeiro(ptr, &reg);
					if(reg.quantidade>0){
							atendente = reg.nAtendente;
							at[atendente].pedidosSucesso++; //aqui ele naopega lixo ne?
							valor =procuraValor(reg.nome,reg.quantidade);
							at[atendente].lucroAtendente+=reg.quantidade * valor;
					}
					pthread_mutex_lock(&t); //Lock para garantir exclusão mutua quando enfileirar.
					desenfileira(ptr); //Desenfileira o primeiro pedido da fila.
					pthread_mutex_unlock(&t); //Unlock do enfileiramento.
		}
		pthread_exit(NULL);
}

//Função main.
int main(int argc, char *argv[] ){
		int rc,i;
		long d;
		pEstoque reg2;

		if( argc != 3 ){
	      printf( "Parametros nao informados!! %i", argc );
	      return 0;
	  }

		//Recebimento do argumentos e atribuição nas variaveis.
	  nthr = atoi(argv[1]);
		nomearq = argv[2];

		//Criação da lista de pedidos.
		if(criaLDDE(&ptr,sizeof(pPedidos))== 0){
				printf ("Erro na criacao do ponteiro para os pedidos\n");
				return 1;
		}

		//Criação da lista de estoque.
		if(criaLDDE(&ptr2,sizeof(pEstoque))==0){
				printf ("Erro na criação do ponteiro para o estoque\n");
				return 1;
		}

		//Alocação da struct atendentes
		at = (struct atendentes *) calloc(nthr, sizeof(struct atendentes));

		//Leitura do arquivo estoque.
		if(leArquivoEstoque() == 1){
				printf("\nErro na leitura do arquivo estoque\n");
		    return 1;
		}

		//Criação e alocação das threads.
		pthread_t *threads;
		threads = (pthread_t *)malloc(sizeof(pthread_t) * (nthr+1));
		if(threads==NULL){
				printf ("Erro na alocação das threads.\n");
				exit(1);
		}

		//Verificação da inicialização correta dos mutexes.
	 	if (pthread_mutex_init(&t, NULL) != 0 || pthread_mutex_init(&t2,NULL)!=0){
				printf("Erro inicializando mutex t\n");
		}

		//Inicialização das barreiras.
		rc = pthread_barrier_init(&barreira, NULL, nthr+1);
		    if (rc != 0) {
		  	perror("erro em pthread_barrier_init()");
		  	exit(1);
	   }

		//Inicialização das threads
		for (i = 0; i < nthr; i++){
		    d=i;
				rc = pthread_create(&threads[i], NULL, atendente_Thread, (void *) d);
				if(rc != 0){
		       		printf ("ERRO - nao fatal = %d\n", rc);
				}
		}

		d=i;
		rc = pthread_create(&threads[i],NULL,caixa_Thread, (void *)d);
		if(rc){
				printf ("ERRO - nao fatal = %d\n", rc);
				exit(-1);
		}

		for (i = 0; i < nthr+1; i++){
	    	rc = pthread_join(threads[i], NULL);
		}

		//Impressao dos relatorios
	  relatorios();

	  // Inicio do arquivo de saida
	  FILE *arqSaida;
	  arqSaida = fopen ("novoarquivo1.txt", "w");
		fprintf (arqSaida, "***** Relatorio financeiro *****\n");
		fprintf (arqSaida,"Atendente \t Pedidos \t Valor(R$)\n");
		for(i=0;i<nthr;i++){
				fprintf (arqSaida, "%6d %10d %12u\n",i+1,at[i].pedidosSucesso, at[i].lucroAtendente);
		}
		fprintf (arqSaida,"\nReceita total: R$ %lu\n",receitatotal);
		fprintf (arqSaida,"\n****** Estoque de lanches *****\n");
		fprintf (arqSaida,"Lanche \t\t\t\t\t   Inicial \t Final\n");
	  for(i=0;i<tamanho(ptr2); i++){
				buscarPosLog(ptr2, &reg2, i);
				fprintf (arqSaida,"%-15s \t %5u %8u\n",reg2.nome, reg2.estoqueInicial,reg2.estoqueAtual);
		}
		fclose(arqSaida);
	    	if (arqSaida != NULL) {
	        	printf ("Dados gravados com sucesso!\n");
	    	}
		//Fim do arquivo de saida

		//Liberacao das memorias
		pthread_cond_destroy(&fimAt);
		pthread_mutex_destroy(&t);
		pthread_mutex_destroy (&t2);
	  free(at);
	  if(destroi(&ptr) == 0 ){
	    	printf("Erro ao destruir a lista de pedidos.\n");
	  }
		if(destroi(&ptr2) == 0 ){
	    	printf("Erro ao destruir a lista de estoque\n");
	  }
		return 0;
}
