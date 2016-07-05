/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Algoritmos e Complexidade
 * Prof. Mario Lizier
 *
 * Trabalho 01 - Fila do Bandejao
 *
 * RA: 511340
 * Aluno: Gabriel Stankevix Soares
 * ========================================================================== */
   
/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
   
/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */
  
void bandejao(int fila[], int distancia[],int N, int K); 
  
/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
  
int main() {
    int N, K,i=0;
    int fila[1000];
    int distancia[1000];
      
   
    while(scanf("%d%d",&N,&K)!=EOF){
         
        fila[0] = 0;
 
        for(i=1;i<N;i++){
            scanf("%d",&fila[i]);
        }
 
        for(i=1;i<N;i++){
            distancia[i] = fila[i] - fila[i-1];
        }
        //fila[N] = -1; //pog
          
        //if(N-1 == K){
            //printf("0\n");
        ///}else{
            bandejao(fila, distancia,N , K);
        //}
    }
    return 0;
}
  
/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */
   
void bandejao(int fila[], int distancia[],int N, int K){
    int i, j, maior = 1,soma = 0;
    int flagDist = 0;
  
    for(i=1; i < K; i++){
        maior = 0; flagDist = 0;
        for(j=1; j < N; j++){
            if(distancia[j] > maior){
                maior = distancia[j];
                flagDist = j;
            }
        }
        distancia[flagDist] = 0;
    }
     
    //printf("flagDist %d\n", flagDist);
    for(i=1;i<N;i++){
        //printf("soma %d\n", soma);
        //printf("distancia[%d] = %d\n", i,distancia[i]);
        soma += distancia[i];
    }
    printf("%d\n", soma);
}
