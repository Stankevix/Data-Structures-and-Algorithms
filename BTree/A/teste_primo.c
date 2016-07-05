/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int prox_primo(int tam){
	int i = 0, j = 0,conta, primo, vetPrimo[1000], contaPrimo = 0;
	
	
	if (tam == 0){
		return 0;
	}else if (tam == 1){
		return 1;
	}

	for (i = 1; i <= 1000; i++){
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
			printf("\nAchei %d\n",vetPrimo[i]);
			return vetPrimo[i];
		}
	}
}

int main (){
	int tam;

	scanf("%d",&tam);
	prox_primo(tam);
	return 0;
}
/*

 int main(){
    int num,i,count;
  
    for(num = 1;num<=100;num++){
         count = 0;

         for(i=2;i<=num/2;i++){
             if(num%i==0){
                 count++;
                 break;
             }
        }
        
         if(count==0 && num!= 1)
             printf("%d ",num);
    }
  
   return 0;
}


*/