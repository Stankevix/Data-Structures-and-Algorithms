/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Algoritmos e Complexidade
 * Prof. Mario Lizier
 *
 * Trabalho 01 - Motoboy
 *
 * RA: 511340
 * Aluno: Gabriel Stankevix Soares
 * ========================================================================== */
  
/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
 /*Variavel Global*/
int tempo[21], quantidade[31], F[21][31], valor;
 
 
/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */
 
int bandejao(int i, int j);
 
int max( int a , int b){
    if(a > b){
        return a;
    }else{
        return b;
    }
} 
 
/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
 
int main() {
    int N, P, i = 0, j = 0;
     
  
    while(1){
        scanf("%d",&N);
 
        if (N == 0){
            return 0;
        }
 
        scanf("%d",&P);
 
        for( i = 0; i < N; i++){
            scanf("%d",&tempo[i]);
            scanf("%d",&quantidade[i]);               
        }
 
        for(i = 0; i < 21; i++){
            for(j = 0; j < 31; j++){
 
                if (i == 0 || j == 0){
                    F[i][j] = 0;
                }else{
                    F[i][j] = -1;
                }
            }
        }
 
 
 
        bandejao(N, P);
 
        printf("%d min.\n", F[N][P]);
    }
    return 0;
}
 
/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */
  
int bandejao(int i, int j){
 
    if(F[i][j] < 0){
        if( j < quantidade[i-1]){
            valor = bandejao(i-1, j);
        }else{
            valor = max(bandejao(i-1,j),
                    tempo[i-1] + bandejao(i-1, j - quantidade[i-1]));
        }
        F[i][j] = valor;
    }
    return F[i][j];
}
