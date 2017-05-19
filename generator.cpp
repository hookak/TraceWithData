#include "box.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned long long ULL;
void wInfo(FILE*,ULL op, ULL lpa, ULL size);

int main(int argc, char* argv[]) {
	//FILE* dataFp, *traceFp;
	FILE* dacFp;
	double entropy, dupRate, pageDiff;
	int entDist, pageDist;
	int nr_dup, pDiff;
	char pDist[10];


//	dataFp = fopen("a.dat", "w");
//	traceFp = fopen("trace.in", "w");
	dacFp = fopen(argv[1], "w");

	//	if( (NULL == dataFp || NULL == traceFp )|| NULL == dacFp) return -1;
	if(argc != 6) {
		printf("Usage : %s [File] [Entropy] [Duplication Rate] [Page Distribution] [Page Difference]\n", argv[0]);
		return -1;
	}
	if(NULL == dacFp) return -1;


	entropy = Rounding(atof(argv[2]),2);
	dupRate = atof(argv[3]);
	pageDist = atoi(argv[4]);
	pageDiff = atof(argv[5]);
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

	for(unsigned long long i=0; i < NR_PAGE; i++) {

		wInfo(dacFp, 1, i*(PAGE_SIZE/512), (PAGE_SIZE/512));
		fwrite(dBox.buf[i], 1, PAGE_SIZE, dacFp);

		/* for data analysis */
		/*
		fwrite(dBox.buf[i], 1, PAGE_SIZE, dataFp);
		fprintf(traceFp, "%3d,%d %3d %10d %12f %6d %2c %4c %9lld + %2lld [swapper/0]\n",8,49,0, 0000, 9.332, 0,\
										'D', 'W', i*(PAGE_SIZE/512), (PAGE_SIZE/512));
		*/
		

		
	}

	unsigned long long idx, pageIdx;

	printf("Generate %d duplicate Pages\n", nr_dup);
	for(int i=0; i < nr_dup; i++) {
		idx = rBox.getIndex(pageDist);
		pageIdx = dBox.dup_idx[idx];
		for(int j=0; j< (PAGE_SIZE/1024); j++) {
			if(rBox.getNumber() <  pDiff)  {
				dBox.cLeftShift(pageIdx, j, (PAGE_SIZE/4));
			}
		}

		wInfo(dacFp, 1, pageIdx*(PAGE_SIZE/512), (PAGE_SIZE/512));
		fwrite(dBox.buf[pageIdx], 1, PAGE_SIZE, dacFp);


		/* for data analysis */
		/*
		fwrite(dBox.buf[pageIdx], 1, PAGE_SIZE, dataFp);
		fprintf(traceFp, "%3d,%d %3d %10d %12f %6d %2c %4c %9lld + %2lld [swapper/0]\n",8,49,0, 0000, 9.332, 0,\
				'D', 'W', pageIdx*(PAGE_SIZE/512), (PAGE_SIZE/512));
		*/

		
	}
	printf("Make %s File\n", argv[1]);

//	fclose(dataFp);
//	fclose(traceFp);
	fclose(dacFp);

	return 0;
}


void wInfo(FILE* fp,ULL op, ULL lpa, ULL size) {
	fwrite(&op, 8,1, fp);
	fwrite(&lpa,8,1, fp);
	fwrite(&size,8,1, fp);
}
