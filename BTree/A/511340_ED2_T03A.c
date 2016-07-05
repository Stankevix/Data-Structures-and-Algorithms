/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 03A - Hashing com reespalhamento linear
 *
 * RA: 511340
 * Aluno: Gabriel Stankevix Soares
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY 7
#define TAM_APELIDO 18
#define TAM_NOME 36
#define TAM_DATA 11
#define TAM_PAIS 21
#define TAM_POSICAO 11
#define TAM_HEROI 21
#define TAM_EQUIPE 21
#define TAM_PARTIDAS 4

#define TAM_REGISTRO 192
#define MAX_REGISTROS 5000
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)

/* Saídas do usuário */
#define OPCAO_INVALIDA "Opcao invalida!\n\n"
#define REGISTRO_N_ENCONTRADO "Registro nao encontrado!\n\n"
#define CAMPO_INVALIDO "Campo invalido! Informe novamente.\n\n"
#define ERRO_PK_REPETIDA "ERRO: Ja existe um registro com a chave primaria: %s.\n\n"
#define ERRO_TABELA_CHEIA "ERRO: Tabela Hash esta cheia!\n\n"
#define REGISTRO_INSERIDO "Registro %s inserido com sucesso. Numero de colisoes: %d.\n\n"

#define POS_OCUPADA 	"[%d] Ocupado: %s\n"
#define POS_LIVRE 		"[%d] Livre\n"
#define POS_REMOVIDA 	"[%d] Removido\n"

/* Estado das posições da tabela hash */
#define LIVRE 0
#define OCUPADO 1
#define REMOVIDO 2

/* Registro do jogador */
typedef struct {
	char pk[TAM_PRIMARY_KEY];
	char apelido[TAM_APELIDO];
	char nome_completo[TAM_NOME];
	char data_nascimento[TAM_DATA];	// DD/MM/AAAA
	char pais[TAM_PAIS];
	char posicao[TAM_POSICAO];		// { offlane, mid, support, carry }
	char heroi1[TAM_HEROI];
	char heroi2[TAM_HEROI];
	char heroi3[TAM_HEROI];
	char equipe[TAM_EQUIPE];
	char partidas_oficiais[TAM_PARTIDAS];
} Jogador;

/* Registro da Tabela Hash
 * Contém o estado da posição, a chave primária e o RRN do respectivo registro */
typedef struct {
	int estado;
	char pk[TAM_PRIMARY_KEY];
	int rrn;
} Chave;

/* Estrutura da Tabela Hash */
typedef struct {
  int tam;
  Chave *v;
} Hashtable;

/* Variáveis globais */
char ARQUIVO[TAM_ARQUIVO];
int nregistros;
/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore();

/* Recebe do usuário uma string simulando o arquivo completo. */
void carregar_arquivo();

/* Exibe o jogador */
void exibir_registro(int rrn);

/* <<< DECLARE AQUI OS PROTOTIPOS >>> */
//calcular o primeiro primo (T) maior ou igual ao valor informado pelo usuario.
int prox_primo(int tam);

//Calcula a posicao exata para insercao na tabela hash
int calculaPosicao(Chave chave, int T);

//Insere registro na tabela hash
int insereTabela(Hashtable *tabela, Chave chave);

//cria e inicializa a tabela hash
void criar_tabela(Hashtable *tabela, int tam);

//carrega a tabela hash conforme o ARQUIVO de registros
void carregar_tabela(Hashtable *tabela);

//funcao que insere um registro no aquivo de dados.
void insereRegristro(int posicao_atual, Chave chave, Jogador jogador);

//funcao que verifica se a data esta nos padroes corretos.
int verifica_data(Jogador jogador);

//realiza o cadastro dos jogadores
void cadastrar(Hashtable *tabela);

//busca uma chave na tabela hash
int busca_chave(Hashtable *tabela, Jogador jogador);

//Funcao altera o numero de partidas de um jogador
void alterar(Hashtable tabela);

//realiza a busca de registros
void buscar(Hashtable tabela);

//Remover node da tabela
void remover(Hashtable *tabela);

//imprimir todos os dados da tabela
void imprimir_tabela(Hashtable tabela);

//libera a tabela
void liberar_tabela(Hashtable *tabela);

//Funcao busca no ARQUIVO um determinado registro
Jogador recuperar_registro(int rrn);

/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main() {

	/* Arquivo */
	int carregarArquivo = 0;
	scanf("%d\n", &carregarArquivo); // 1 (sim) | 0 (nao)
	if (carregarArquivo) carregar_arquivo();



	/* Tabela Hash */
	int tam;
	scanf("%d", &tam);
	tam = prox_primo(tam);

	Hashtable tabela;
	criar_tabela(&tabela, tam);
	if (carregarArquivo) carregar_tabela(&tabela);



	/* Execução do programa */
	int opcao = 0;
	while(opcao != 6) {
		scanf("%d", &opcao);
		switch(opcao) {

		case 1:
			getchar();
			cadastrar(&tabela);
			break;
		case 2:
			getchar();
			alterar(tabela);
			break;
		case 3:
			getchar();
			buscar(tabela);
			break;
		case 4:
			remover(&tabela);
			break;
		case 5:
			imprimir_tabela(tabela);
			break;
		case 6:
			liberar_tabela(&tabela);
			break;

		case 10:
			printf("%s\n", ARQUIVO);
			break;

		default:
			ignore();
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}



/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/* Recebe do usuário uma string simulando o arquivo completo. */
void carregar_arquivo() {
	scanf("%[^\n]\n", ARQUIVO);
}

/* Exibe o jogador */
void exibir_registro(int rrn) {

	Jogador j = recuperar_registro(rrn);

	printf("%s\n", j.pk);
	printf("%s\n", j.apelido);
	printf("%s\n", j.nome_completo);
	printf("%s\n", j.data_nascimento);
	printf("%s\n", j.pais);
	printf("%s\n", j.posicao);
	printf("%s\n", j.heroi1);
	printf("%s\n", j.heroi2);
	printf("%s\n", j.heroi3);
	printf("%s\n", j.equipe);
	printf("%s\n", j.partidas_oficiais);
	printf("\n");
}


/* <<< IMPLEMENTE AQUI AS FUNCOES >>> */

//calcular o primeiro primo (T) maior ou igual ao valor informado pelo usuario.
int prox_primo(int tam){
	int i = 0, j = 0,conta, vetPrimo[6000], contaPrimo = 0;
	
	if (tam == 0){
		return 0;
	}else if (tam == 1){
		return 1;
	}

	for (i = 1; i <= 6000; i++){
		conta = 0;
		for (j = 2; j<=i/2;j++){
			if(i % j == 0){
				conta++;
			}
		}
		if (conta == 0 && i!= 1){
			vetPrimo[contaPrimo] = i;
			contaPrimo++;
		}
	}

	for(i=0;i<contaPrimo;i++){
		if (vetPrimo[i] >= tam){
			return vetPrimo[i];
		}
	}
	return 0;
}

//cria e inicializa a tabela hash
void criar_tabela(Hashtable *tabela, int tam){
	int i = 0;

	tabela->tam = tam;
	tabela->v = (Chave*) malloc((tam)* sizeof(Chave));

	for(i = 0; i < tam; i++){
		tabela->v[i].estado = LIVRE;
	}

}

int calculaPosicao(Chave chave, int T){
	int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0, h = 0;

	k1 = (int)chave.pk[0];
	k2 = (int)chave.pk[1];
	k3 = (int)chave.pk[2];
	k4 = (int)chave.pk[3];
	k5 = (int)chave.pk[4];
	k6 = (int)chave.pk[5];

	h = h + (1 * k1);
	h = h + (2 * k2);
	h = h + (3 * k3);
	h = h + (4 * k4);
	h = h + (5 * k5);
	h = h + (6 * k6);

	h = h % T;

	return h;
}
//Insere registro na tabela hash
int insereTabela(Hashtable *tabela, Chave chave){
	int h = 0, i = 0, num_colisao = 0;

	h = calculaPosicao(chave, tabela->tam);

	/*if(tabela->v[h].estado == REMOVIDO){
		h++;
	}*/

	if (tabela->v[h].estado == LIVRE){
		tabela->v[h] = chave;
		tabela->v[h].estado = OCUPADO;
		return num_colisao;

	}else if (tabela->v[h].estado == OCUPADO || tabela->v[h].estado == REMOVIDO){
		
		//if(tabela->v[h].estado == OCUPADO){
			num_colisao++;
		//}

		for (i = h+1; i < tabela->tam; i++){
			if(tabela->v[i].estado == LIVRE){
				tabela->v[i] = chave;
				tabela->v[i].estado = OCUPADO;
				return num_colisao;
			}

			//if(tabela->v[i].estado == OCUPADO){
				num_colisao++;	
			//}
		}

		//Caso exista uma posicao no inicio da fila que possa ser preenchida.
		for (i = 0; i < tabela->tam; i++){
			if(tabela->v[i].estado == LIVRE){
				tabela->v[i] = chave;
				tabela->v[i].estado = OCUPADO;
				return num_colisao;
			}

			//if(tabela->v[i].estado == OCUPADO){
				num_colisao++;	
			//}
		}

		return num_colisao;
	}

	return num_colisao;
}


//carrega a tabela hash conforme o ARQUIVO de registros
void carregar_tabela(Hashtable *tabela){
	int cont = 0;
	char *p= ARQUIVO + 0;
	Chave chave;

	nregistros = (strlen(ARQUIVO))/TAM_REGISTRO;
	
	while(cont < nregistros){
		p = ARQUIVO + (cont*TAM_REGISTRO);
		sscanf(p,"%6s",chave.pk);
		chave.rrn = TAM_REGISTRO * cont;
		
		insereTabela(tabela,chave);
		
		cont++;
	}
}


//funcao que insere um registro no aquivo de dados.
void insereRegristro(int posicao_atual, Chave chave, Jogador jogador){
	int cont=0;
	int flagInicio = posicao_atual;

	while(cont!=strlen(chave.pk)){
		ARQUIVO[posicao_atual] = chave.pk[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.apelido)){
		ARQUIVO[posicao_atual] = jogador.apelido[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.nome_completo)){
		ARQUIVO[posicao_atual] = jogador.nome_completo[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.data_nascimento)){
		ARQUIVO[posicao_atual] = jogador.data_nascimento[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.pais)){
		ARQUIVO[posicao_atual] = jogador.pais[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.posicao)){
		ARQUIVO[posicao_atual] = jogador.posicao[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;


	while(cont!=strlen(jogador.heroi1)){
		ARQUIVO[posicao_atual] = jogador.heroi1[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.heroi2)){
		ARQUIVO[posicao_atual] = jogador.heroi2[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.heroi3)){
		ARQUIVO[posicao_atual] = jogador.heroi3[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.equipe)){
		ARQUIVO[posicao_atual] = jogador.equipe[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	while(cont!=strlen(jogador.partidas_oficiais)){
		ARQUIVO[posicao_atual] = jogador.partidas_oficiais[cont];
		cont++;
		posicao_atual++;
	}
	cont = 0;
	ARQUIVO[posicao_atual] = '@';
	posicao_atual++;

	//completa o Arquivo de dados com hashtag
	for (cont = (posicao_atual - flagInicio); cont < TAM_REGISTRO; cont++){
		ARQUIVO[posicao_atual] = '#';
		posicao_atual++;
	}
}


//funcao que valida a data conforme a especificacao
int verifica_data(Jogador jogador){
    if ( jogador.data_nascimento[0] > '3' || (jogador.data_nascimento[0] <= '0' && jogador.data_nascimento[1] <= '0')){
        printf(CAMPO_INVALIDO);
        return 0;
    }else if ( jogador.data_nascimento[0] > '3' && jogador.data_nascimento[1] > '1'){
        printf(CAMPO_INVALIDO);
        return 0;
    }else if(jogador.data_nascimento[3] > '1'|| (jogador.data_nascimento[3]== '1' && jogador.data_nascimento[4] > '2')){
        printf(CAMPO_INVALIDO);
        return 0;
    }

        //verificacao ano minimo 1914
    else if (jogador.data_nascimento[6] == '0' || (jogador.data_nascimento[6] == '1' && jogador.data_nascimento[7] < '9')){
        printf(CAMPO_INVALIDO);
        return 0;
    }else if((jogador.data_nascimento[6] == '1') && (jogador.data_nascimento[7] == '9') && (jogador.data_nascimento[8] <='1') && (jogador.data_nascimento[9] < '4') ){
        printf(CAMPO_INVALIDO);
        return 0;
    }

    //verificacao ano maximo 2002
    else if(jogador.data_nascimento[6] == '0' || jogador.data_nascimento[6] > '2' || (jogador.data_nascimento[6] == '2' && jogador.data_nascimento[7] > '0')){
        printf(CAMPO_INVALIDO);
        return 0;
    }else if((jogador.data_nascimento[6] == '2' && jogador.data_nascimento[7] == '0' && jogador.data_nascimento[8] > '0')){
        printf(CAMPO_INVALIDO);
        return 0;
    }else if ( (jogador.data_nascimento[6] == '2') && (jogador.data_nascimento[7] == '0') && (jogador.data_nascimento[8] =='0') && (jogador.data_nascimento[9] > '2') ){
        printf(CAMPO_INVALIDO);
        return 0;
    }

    else{
        return 1;
    }
}

int busca_chave(Hashtable *tabela, Jogador jogador){
	int i =0,achou = 0,rrn=0;

	for (i = 0; i < tabela->tam; i++){
		if(tabela->v[i].estado == OCUPADO){
			if(strcmp(tabela->v[i].pk,jogador.pk) == 0){
				achou++;
				rrn = tabela->v[i].rrn;
				return rrn;
			}
		}
	}

	if (achou == 0){
		return (-1);
	}
	return -1;
}

//realiza o cadastro dos jogadores
void cadastrar(Hashtable *tabela){
	Jogador jogador;
	Chave chave;
	int flagPartidas=0, flagData=0, i = 0, flagRepetido = 0, posicao_relativa = 0,colisao = 0;

	scanf("%18[^\n]",jogador.apelido);
	ignore();

	scanf("%36[^\n]",jogador.nome_completo);
	ignore();
	    
	/*FAzer verificação de data*/
	while(flagData!=1){
	    scanf("%11[^\n]",jogador.data_nascimento);
	 	ignore();
	    if (jogador.data_nascimento[2]!='/' || jogador.data_nascimento[5]!='/'){//garante que a escrita da data seja dd/mm/aaaa
	        printf(CAMPO_INVALIDO);
	    }
	    else if (verifica_data(jogador) == 1){
	        flagData = 1;
	    }
	}

	scanf("%21[^\n]",jogador.pais);
	ignore();

	scanf("%11[^\n]",jogador.posicao);
	ignore();

	scanf("%21[^\n]",jogador.heroi1);
	ignore();
	   
	scanf("%21[^\n]",jogador.heroi2);
	ignore();   

	scanf("%21[^\n]",jogador.heroi3);
	ignore();

	scanf("%21[^\n]",jogador.equipe);
	ignore();

	while(flagPartidas!=1){
	    scanf("%4[^\n]",jogador.partidas_oficiais);
	    ignore();
	    if (strlen(jogador.partidas_oficiais ) > 3){
	        printf(CAMPO_INVALIDO);

	    }else if ((jogador.partidas_oficiais[0] >= 'a' && jogador.partidas_oficiais[0] <='z') || (jogador.partidas_oficiais[1] >= 'a' && jogador.partidas_oficiais[1] <='z') || (jogador.partidas_oficiais[2] >= 'a' && jogador.partidas_oficiais[2] <='z')){
	        printf(CAMPO_INVALIDO);
	    }else if ((jogador.partidas_oficiais[0] >= 'A' && jogador.partidas_oficiais[0] <='Z') || (jogador.partidas_oficiais[1] >= 'A' && jogador.partidas_oficiais[1] <='Z') || (jogador.partidas_oficiais[2] >= 'A' && jogador.partidas_oficiais[2] <='Z')){
	        printf(CAMPO_INVALIDO);
	    }else{
	        flagPartidas = 1;
	    }
	}
	
	    
	chave.pk[0] = jogador.apelido[0];
	chave.pk[1] = jogador.apelido[1];
	chave.pk[2] = jogador.equipe[0];
	chave.pk[3] = jogador.equipe[1];
	chave.pk[4] = jogador.data_nascimento[8];
	chave.pk[5] = jogador.data_nascimento[9];
	chave.pk[6] = '\0';


	for(i=0; i<strlen(chave.pk); i++){
	    chave.pk[i] = toupper(chave.pk[i]);
	}
	strcpy(jogador.pk,chave.pk);
	chave.rrn = nregistros * TAM_REGISTRO;

	//Verifica se a chave ja existe na arvore
	flagRepetido = busca_chave(tabela, jogador);

	if ( flagRepetido == (-1)){

		if(nregistros >= tabela->tam){
			printf(ERRO_TABELA_CHEIA);
			return;
		}
		nregistros++;

		//chave.rrn = nregistros * TAM_REGISTRO;
		colisao = insereTabela(tabela, chave);
		
		insereRegristro(chave.rrn,chave,jogador);
		printf(REGISTRO_INSERIDO, chave.pk,colisao);

	    }else{
	    	printf(ERRO_PK_REPETIDA,chave.pk);
	    }
}



//Funcao altera o numero de partidas de um jogador
void alterar(Hashtable tabela){
	Jogador jogador;
	int flagPartidas=0,rrn=0,i=0,contaArroba=0;

	scanf("%6[^\n]",jogador.pk);
	ignore();

	rrn = busca_chave(&tabela, jogador);


	while(flagPartidas!=1){
    	scanf("%4[^\n]",jogador.partidas_oficiais);
    	ignore();
        if (strlen(jogador.partidas_oficiais ) > 3 || strlen(jogador.partidas_oficiais ) == 1 || strlen(jogador.partidas_oficiais ) == 2 || strlen(jogador.partidas_oficiais ) == 0){
            printf(CAMPO_INVALIDO);

        }else if ((jogador.partidas_oficiais[0] >= 'a' && jogador.partidas_oficiais[0] <='z') || (jogador.partidas_oficiais[1] >= 'a' && jogador.partidas_oficiais[1] <='z') || (jogador.partidas_oficiais[2] >= 'a' && jogador.partidas_oficiais[2] <='z')){
            printf(CAMPO_INVALIDO);
        }else if ((jogador.partidas_oficiais[0] >= 'A' && jogador.partidas_oficiais[0] <='Z') || (jogador.partidas_oficiais[1] >= 'A' && jogador.partidas_oficiais[1] <='Z') || (jogador.partidas_oficiais[2] >= 'A' && jogador.partidas_oficiais[2] <='Z')){
            printf(CAMPO_INVALIDO);
        }else{
            flagPartidas = 1;
        }
    }

    if(rrn == -1){
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}
    
    for (i = rrn;i < (rrn+TAM_REGISTRO);i++){
    	if (ARQUIVO[i] == '@'){
    		contaArroba++;
    	}
    	if(contaArroba == 10){
    		ARQUIVO[i+1] = jogador.partidas_oficiais[0];
    		ARQUIVO[i+2] = jogador.partidas_oficiais[1];
    		ARQUIVO[i+3] = jogador.partidas_oficiais[2];
    		return;
    	}
    }
}

//realiza a busca de registros
void buscar(Hashtable tabela){
	int rrn = 0, i = 0, achou = 0;
	Jogador jogador;
	
	scanf("%6[^\n]",jogador.pk);
	ignore();
	
	rrn = busca_chave(&tabela, jogador);

	if (rrn != (-1)){
		exibir_registro(rrn);
	}else{
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}

}

//Remover node da tabela
void remover(Hashtable *tabela){
	int rrn = 0, i = 0;
	Jogador jogador;

	scanf("%*c");
	scanf("%6[^\n]",jogador.pk);
	ignore();

	rrn = busca_chave(tabela, jogador);

	if(rrn == -1){
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}

	for (i=0;i<tabela->tam;i++){
		if(tabela->v[i].rrn == rrn){
			tabela->v[i].estado = REMOVIDO;
		}
	}

	ARQUIVO[rrn] = '*';
	ARQUIVO[rrn+1] = '|';
}

//imprimir todos os dados da tabela
void imprimir_tabela(Hashtable tabela){
	int i =0;

	for(i=0;i<tabela.tam;i++){
		if(tabela.v[i].estado == REMOVIDO){
			printf("[%d] Removido\n",i);
		}else if (tabela.v[i].estado == LIVRE){
			printf("[%d] Livre\n",i);
		}else{
			printf("[%d] Ocupado: %s\n",i,tabela.v[i].pk);
		}
	}
}

//libera a tabela
void liberar_tabela(Hashtable *tabela){
	int i = 0;

}

Jogador recuperar_registro(int rrn){
	Jogador jogador;
	char *p = ARQUIVO + rrn;//ponteiro para a posicao relativa ao rrn

	sscanf(p,"%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@",jogador.pk,jogador.apelido,jogador.nome_completo,jogador.data_nascimento,jogador.pais,jogador.posicao,jogador.heroi1,jogador.heroi2,jogador.heroi3,jogador.equipe,jogador.partidas_oficiais);
	return jogador;
}
