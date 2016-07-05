/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Teoria dos Grafos
 * Prof. Candida Nunes da Silva
 *
 * Trabalho 3 - 'Caminhos'
 *
 * RA: 511340
 * Aluno: Gabriel Stankevix Soares
 * ========================================================================== */

 /* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Registros*/

//Registro para os nodes
typedef struct no No;
struct no{
	int valor;
	int cor;
	int pred;
	int descoberta;
	int finalizacao;
	int peso;
	No *prox;
};

//Registro para os nos cabeca
typedef struct{
    int valor;
    No **nodes;
}Grafo;

//Estrutura para Fila
typedef struct Ap_fila{
  No* head;
  No* tail;
} Ap_fila;

typedef Ap_fila Fila;

//VAriaveis Globais
int explorados[5000];
int contaExplorados=0;
int somaAtual = 0;
int somaMax = 0;
/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

//cria grafo(vetor de nos cabeças)
void cria_grafo(Grafo *grafo, int N);

//cria um novo node na lista de adjacencia.
No *cria_node(int num);

//insere arestas no grafo;
int insere_aresta(Grafo *grafo, int u, int v);

//Remover grafo alocadao
void remover_grafo (Grafo *grafo);

//Monta o grafo junamente com suas realações de vertices e arestas
void montaGrafo(int N,int M);

//Primeira etapa da busca
int DFS(Grafo *grafo);

//Realiza as operacoes de busca
void DFS_AUX(Grafo *grafo, int u, int *tempo);

void achaSoma(GRAFO *grafoTransposto,int u);

/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * ========================================================================== */

int main(){
	int N,M;//N reprensenta o numero de pessoas e M o numero de relacoes

	scanf("%d %d", &N,&M);
	while (N!=0){
		
		if(!M){
			printf("0\n");
		}else{
			montaGrafo(N,M);
		}
		scanf("%d %d", &N,&M);
	}
	return 0;
}

/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */


//primeira etapa da busca
int DFS(Grafo *grafo){
	int i=0, tempo=0,maior=0;

	for(i=0;i<grafo->valor;i++){
		if(grafo->nodes[i]->cor == 1){
			DFS_AUX(grafo,i,&tempo);
		}
	}
	return 1;
}

//Realiza as operacoes de busca
void DFS_AUX(Grafo *grafo, int u, int *tempo){
	No *aux;
	int v;

	grafo->nodes[u]->cor = 0;
	grafo->nodes[u]->descoberta= *tempo; 
	(*tempo)++;

	aux = grafo->nodes[u]->prox;
	while(aux!=NULL){
		v = aux->valor;
		if(grafo->nodes[v]->cor == 1){
			grafo->nodes[v]->pred = u;
			DFS_AUX(grafo, v, tempo);
		}
		aux = aux->prox;
	}

	grafo->nodes[u]->cor = -1;
	grafo->nodes[u]->finalizacao = (*tempo);
	(*tempo)++;

	explorados[contaExplorados] =  grafo->nodes[u]->valor;
	contaExplorados++;
}

//A soma correta no vertor de explorados
void achaSoma(GRAFO *grafoTransposto,int u){
	No *aux;
	int v;

	grafoTransposto->nodes[u]->cor = 0;
	somaAtual += grafoTransposto->nodes[u]->peso;

	aux = grafoTransposto->nodes[u]->prox;
	while(aux!=NULL){
		v = aux->valor;
		somaAtual += grafoTransposto->nodes[v]->peso;
		aux = aux->prox;
	}

	if(somaAtual >= somaMax){
		somaMax = somaAtual;
	}

}



/**** OPERACOES DO GRAFO ********/

//cria as listas de adjacencias para o grafo 
void cria_grafo(Grafo *grafo,int N){
	int i = 0;

	grafo->valor = N;
	grafo->nodes = (No**) malloc((N) * sizeof(No*));

	for(i = 0; i < N; i++){
		grafo->nodes[i] = malloc (N * sizeof(No));
		grafo->nodes[i]->valor = i;
		grafo->nodes[i]->cor = 1;
		grafo->nodes[i]->pred = -1;
		grafo->nodes[i]->descoberta = 0;
		grafo->nodes[i]->finalizacao = 0;
		grafo->nodes[i]->peso = 0;
		grafo->nodes[i]->prox = NULL;
	}
}

//cria um novo node de chave 
No *cria_node(int num){
	No *novo;

	novo = (No*) malloc (sizeof(No));
	novo->valor = num;
	novo->cor = 1;
	novo->pred = -1;
	novo->descoberta = 0;
	novo->finalizacao = 0;
	novo->peso = 0;
	novo->prox = NULL;

	return novo;
}

//insere arestas no grafo;
int insere_aresta(Grafo *grafo, int u, int v){
	No *aux,*novo;

	aux = grafo->nodes[u];
	//trocar para inserir no inicio
	//while(aux->prox != NULL){
		//aux = aux->prox;
	//}

	novo = (No*) malloc (sizeof(No));
	novo->valor = v;
	novo->cor = 1;
	novo->pred = -1;
	novo->descoberta = 0;
	novo->finalizacao = 0;
	novo->peso = 0;
	//novo->prox = NULL;

	novo->prox = aux->prox;
	aux->prox = novo;
	
	return 1;
}


//Remover grafo alocadao
void remover_grafo (Grafo *grafo){
	int i = 0;

	for( i = 0 ; i < grafo->valor; i++)
    	free(grafo->nodes[i]);
	free( grafo->nodes);
}

/*
branco = 1,
cinza = 0,
preto = -1
*/

//Monta o grafo junamente com suas realações de vertices e arestas
void montaGrafo(int N,int M){
	 int i = 0, u = 0, v = 0, peso[N], tempo = 0, noAtual = 0;
	 Grafo grafo;
	 Grafo grafoTransposto;
	 somaMax = 0;

	 cria_grafo(&grafo,N);
	 cria_grafo(&grafoTransposto,N);

	//ler pesos
	while(i<N){
	 	scanf("%d",&peso[i]);
	 	i++;
	 }
	
	//Definir aresta do grafo e grafo transposto 
	i = 0;
	while(i<M){
	 	scanf("%d %d", &u, &v);
	 	insere_aresta(&grafo, u, v);
	 	insere_aresta(&grafoTransposto,v,u);
	 	i++;
	 }

	//Adicionar pesos aos nodes de ambos os grafos.
	i=0;
	while(i<N){
	 	grafo.nodes[i]->peso = peso[i];
	 	grafoTransposto.nodes[i]->peso = peso[i];
	 	i++;
	 }

	 //montar funcao de buscar em profundidade DFS
	 DFS(&grafo); 

	 //Usa vetor de ordenacao topologica para achar o caminho/soma
	 i = 0;
	 for(i=contaExplorados-1;i>=0;i--){
	 	somaAtual = 0;
	 	noAtual = explorados[i];
	 	achaSoma(grafoTransposto, noAtual);
	 }

	 printf("%d\n",somaMax);

	 remover_grafo(&grafo);
	 remover_grafo(&grafoTransposto);

	 contaExplorados = 0;
}


