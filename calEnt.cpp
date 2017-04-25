#include "ent.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

double calEnt(unsigned char* d, double* bufEnt) {
	double page_ent = 0.0;
	long long page_ch[256]; //cal Ent for 4K page
	long long data_ch[256]={0}; //cal Ent for hole data
	double temp, cal;


	memset(page_ch, 0, sizeof(long long)*256);
	page_ent = 0.0;
	for(int j=0; j< PAGE_SIZE/8; j++) {
		unsigned char idx = d[j];

		page_ch[idx]++;
		data_ch[idx]++;
	}

	for(int j=0; j<256; j++) {
		temp = (double)page_ch[j]/(PAGE_SIZE/8);
		if(page_ch[j]==0)
			page_ent -= 0;
		else
			page_ent -= temp*log2(temp);
		// 	printf("cal %lf\n", cal);

		//hist[int(cal*100)]++;

	}
	cal = Rounding((page_ent/8),2);

	/*
	   for(int i=0; i <256; i++) {
	   temp = (double)data_ch[i]/((PAGE_SIZE*NR_PAGE)/8);
	   if(data_ch[i]==0)
	   data_ent -= 0;
	   else
	   data_ent -= temp*log2(temp);

	   }
	 */

	return cal;

}
