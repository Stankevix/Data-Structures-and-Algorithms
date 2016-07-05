/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Algoritmos e Complexidade
 * Prof. Mario Lizier
 *
 * Trabalho 01 - Lucro
 *
 * RA: 511340
 * Aluno: Gabriel Stankevix Soares
 * ========================================================================== */
 
/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 
//Desta forma ele pode lucrar (35 + 80 + 6) - (N * custoPorDia) = $ 61.
 
/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */
 
void calculaLucro(int receita [], int N, int custoPorDia);
  
/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main() {
    int N = 0, custoPorDia = 0, i = 0;
    int receita[1000];
 
    while(scanf("%d",&N)!=EOF){
         
        scanf("%d",&custoPorDia);
     
        for (i = 0; i < N; i++){
            scanf("%d",&receita[i]);
        }
         
        if (N == 0){
            printf("0\n");
        }else{
            calculaLucro(receita, N , custoPorDia);
        }
    }
    return 0;
}
 
 
 
/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */
 
void calculaLucro(int receita [], int N, int custoPorDia){
    int i = 0, j = 0, somaAtual = 0, receitaAtual = 0,vetorSoma[N], maior = 0, dias = 0;
 
    //maior = ( receita[0]  - (1 * custoPorDia));
    //receitaAtual = maior;
    for(i = 0; i < N; i++){
        somaAtual = receita[i];
        dias = 1;
         
        receitaAtual = somaAtual  - ( dias * custoPorDia);
        if(receitaAtual > maior){
                maior = receitaAtual;
        }
 
        for( j = i + 1; j < N ; j++){
            somaAtual = somaAtual + receita[j];
            dias++;
            receitaAtual = somaAtual  - ( dias * custoPorDia);
            if(receitaAtual > maior){
                maior = receitaAtual;
            }
        }
    }
    if(maior <= 0){
        printf("0\n");
    }else{
        printf("%d\n",maior);
    }
}
