/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main (){
	char str [7];
	int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0, h = 0;

	scanf("%s",str);

	k1 = (int)str[0];
	k2 = (int)str[1];
	k3 = (int)str[2];
	k4 = (int)str[3];
	k5 = (int)str[4];
	k6 = (int)str[5];

	printf("\n k1 %d\n",k1);
	printf("\n k2 %d\n",k2);
	printf("\n k3 %d\n",k3);
	printf("\n k4 %d\n",k4);
	printf("\n k5 %d\n",k5);
	printf("\n k6 %d\n",k6);

	h = h + (1 * k1);
	h = h + (2 * k2);
	h = h + (3 * k3);
	h = h + (4 * k4);
	h = h + (5 * k5);
	h = h + (6 * k6);
	
	h = h % 10;

	printf("\n h %d\n",h);

return 0;

}