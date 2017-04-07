#include "ent.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

double calEnt(char* fileName) {
	double entropy = 0.0;
	FILE* fp;
	long long alphabet[256];
	long long fileSize = 0;
	unsigned char buf[BUFSIZE];
	int i, n;

	if((fp = fopen(fileName, "r"))== NULL) {
		printf("'%s' open error\n", fileName);
		return -1;
	}

	memset(alphabet, 0, sizeof(long long)*256);

	while((n = fread(buf, 1, BUFSIZE, fp)) != 0)
	{
		for( i=0; i<n; i++) {
			alphabet[(int)buf[i]]++;
			fileSize++;
		}
	}
	fclose(fp);

	
	double temp;
	for( i=0; i<256; i++) {
		temp = (double)alphabet[i]/fileSize;
		if(alphabet[i]==0)
			entropy -= 0;
		else
			entropy -= temp*log2(temp);

	}

	/*
	printf("%s\n",fileName);
	printf("Entropy = %02.7f bits per byte\n",entropy);
	printf("Entropy = %02.7f bits per bit\n", entropy/8);
	*/
	return entropy;

}
