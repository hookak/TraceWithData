#include "box.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char* argv[]) {
	FILE* dataFp, *traceFp;
	double entropy, dupRate, pageDiff;
	int entDist, pageDist;
	int nr_dup, pDiff;
	char pDist[10];


	dataFp = fopen("a.dat", "w");
	traceFp = fopen("trace.in", "w");

	if( NULL == dataFp || NULL == traceFp) return -1;
	if(argc != 5) {
	printf("Usage : %s [Entropy] [Duplication Rate] [Page Distribution] [Page Difference]\n", argv[0]);
		return -1;
	}

	entropy = Rounding(atof(argv[1]),2);
	dupRate = atof(argv[2]);
	pageDist = atoi(argv[3]);
	pageDiff = atof(argv[4]);
	entDist = 1; 
	nr_dup = (NR_PAGE*dupRate)/(1.0 - dupRate);
	if(pageDist == 0) strcpy(pDist, "UNIFORM");
	else if(pageDist == 1) strcpy(pDist, "NORMAL");
	pDiff = pageDiff*10000;

	printf("Entropy : %f Duplication Rate : %f, Page Distribution : %s, Page Difference : %f\n" \
			, entropy, dupRate,pDist, pageDiff);

	random_box rBox(entropy);
	data_box dBox(entropy);

	dBox.genData(rBox, entDist);
	printf("Generate %d Pages\n", NR_PAGE);

	for(int i=0; i < NR_PAGE; i++) {
		fwrite(dBox.buf[i], 1, PAGE_SIZE, dataFp);
		fprintf(traceFp, "%3d,%d %3d %10d %12f %6d %2c %4c %9d + %2d [swapper/0]\n",8,49,0, 0000, 9.332, 0,\
										'D', 'W', i*(PAGE_SIZE/512), (PAGE_SIZE/512));
	}

	int idx;

	printf("Generate %d duplicate Pages\n", nr_dup);
	for(int i=0; i < nr_dup; i++) {
		idx = rBox.getIndex(pageDist);

		for(int j=0; j< (PAGE_SIZE/1024); j++) {
			if(rBox.getNumber() <  pDiff) 
				dBox.cLeftShift(idx, j, (PAGE_SIZE/4));
		}

		fwrite(dBox.buf[idx], 1, PAGE_SIZE, dataFp);
		fprintf(traceFp, "%3d,%d %3d %10d %12f %6d %2c %4c %9d + %2d [swapper/0]\n",8,49,0, 0000, 9.332, 0,\
				'D', 'W', idx*(PAGE_SIZE/512), (PAGE_SIZE/512));

	}

	return 0;
}
