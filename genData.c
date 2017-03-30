#include "ent.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


//px Probability of 1
double px[11] = {0.0, 0.9870, 0.9688, 0.9467, 0.9206
		   	, 0.8899, 0.8538, 0.8107, 0.7569, 0.6839, 0.5000};


int genData(char* fileName, double entropy) {
	int ent = (int)(entropy*10);
	int pInt =(int)(px[ent]*10000);
	int idx = 0;
	char buf[BUFSIZE];
	FILE *fp;

	if((fp = fopen(fileName, "w")) == NULL) {
		printf("'%s' open error\n", fileName);
		return -1;
	}
	srand(time(NULL));

	for(int page=0; page < NR_PAGE; page++) {
		memset(buf, 0, BUFSIZE);
		idx = 0;
		for(int i=0; i< BUFSIZE; i++) {
			char bit = 0x01;
			for(int j=0; j<8; j++) {
				int n =rand()%10000;
				if(n < pInt) {
					buf[idx] +=bit;
				}
				bit = bit<<1;
			}
			idx++;
		}
		fwrite(buf, 1, BUFSIZE, fp);
	}
	fclose(fp);
	return 0;
}
