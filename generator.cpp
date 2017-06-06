#include "box.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

typedef unsigned long long ULL;
void wInfo(FILE*,ULL op, ULL lpa, ULL size);


class infoLBA {
	public:
	ULL lba, nr_cnt, curSeq, totalSeq;
	vector<pair<ULL,ULL>> seq;

	infoLBA(ULL l) {
		lba = l;
		nr_cnt=0;
		curSeq=0;
		totalSeq = 0;
	}
};


bool comp(const pair<ULL,ULL> a, const pair<ULL,ULL> b) { return a.second > b.second;}
bool comp1(const infoLBA a, infoLBA b) {return a.nr_cnt > b.nr_cnt;}

vector<pair<ULL,ULL>> genV;
vector<infoLBA> inputV;
vector<ULL> matchIdx;

int main(int argc, char* argv[]) {
	//FILE* dataFp, *traceFp;
	FILE* dacFp;
	FILE* lbaFp;
	double entropy, pageDiff;
	int entDist, pageDist;
	int  pDiff;
	char pDist[10];
	ULL maxLBA=0, total_4K_write_reqs=0;


//	dataFp = fopen("a.dat", "w");
//	traceFp = fopen("trace.in", "w");
	dacFp = fopen(argv[1], "w");
	lbaFp = fopen(argv[2], "r");

	//	if( (NULL == dataFp || NULL == traceFp )|| NULL == dacFp) return -1;
	if(argc != 6) {
		printf("Usage : %s [Output File] [LBA File] [Entropy] [Page Distribution] [Page Difference]\n", argv[0]);
		return -1;
	}
	if(NULL == dacFp) {return -1;}
	if(NULL == lbaFp) {return -1;}

	entropy = Rounding(atof(argv[3]),2);
	pageDist = atoi(argv[4]);
	pageDiff = atof(argv[5]);
	entDist = 1; 
	if(pageDist == 0) strcpy(pDist, "UNIFORM");
	else if(pageDist == 1) strcpy(pDist, "NORMAL");
	pDiff = pageDiff*10000;

	printf("Entropy : %f, Page Distribution : %s, Page Difference : %f\n" \
			, entropy,pDist, pageDiff);

	random_box rBox(entropy);
	data_box dBox(entropy);
	dBox.genData(rBox, entDist);



	/* lba.in File read */
	ULL inputLBA, inputSize, inputOp;
	ULL pageIdx, idx;

	while(!feof(lbaFp)) {
		fscanf(lbaFp, "%lld %lld %lld", &inputOp, &inputLBA, &inputSize);
		for(ULL i=0; i< inputSize; i++) {
			if(maxLBA < (inputLBA + i)) maxLBA = (inputLBA+i);
		}
		if(inputOp == 1) total_4K_write_reqs += inputSize; //write Request
	}
	fseek(lbaFp, 0, SEEK_SET);
	printf("MaxLBA : %lld, Total 4K Write req :  %lld\n", maxLBA, total_4K_write_reqs);

	for(int i=0; i< NR_PAGE; i++) genV.push_back(make_pair(i,0));
	for(ULL i=0; i<= maxLBA ; i++) {
		inputV.push_back(i);
		matchIdx.push_back(0);
	}
	/* make lba sequence */
	for(ULL i=0; i < total_4K_write_reqs; i++) {
		idx = rBox.getIndex(pageDist);
		pageIdx = dBox.dup_idx[idx];
		genV[pageIdx].second++;
	}

	//int middle = dBox.dup_idx[NR_PAGE/2];

	sort(genV.begin(), genV.end(), comp);

	/*
	for(int i=0; i< NR_PAGE; i++) {
		printf("%lld %lld\n", genV[i].first, genV[i].second);
	}
*/
	while(!feof(lbaFp)) {
		fscanf(lbaFp, "%lld %lld %lld",&inputOp, &inputLBA, &inputSize);
		if(inputOp==1) {
			for(ULL i=0; i< inputSize; i++)
				inputV[inputLBA+i].nr_cnt++;
		}
	}
	sort(inputV.begin(), inputV.end(), comp1);

	/* matchIdx */
	for(ULL i=0; i < maxLBA; i++) {
		ULL lba = inputV[i].lba;
		matchIdx[lba] = i;
	}

	ULL inIdx=0;
	for(int i=0; i < NR_PAGE; i++) {
		ULL lba = genV[i].first;
		ULL& cnt = genV[i].second;
		while(1) {
			ULL nr_cnt = inputV[inIdx].nr_cnt;
			if( nr_cnt  <= cnt) {

				inputV[inIdx].seq.push_back(make_pair(lba, nr_cnt));
				inputV[inIdx].nr_cnt = 0;
				inputV[inIdx].totalSeq++;
				cnt -= nr_cnt;
			//	printf("lba : %lld, Cnt : %lld\n",lba, cnt);
				inIdx++;
				if(cnt ==0) break;
			} else {
				inputV[inIdx].seq.push_back(make_pair(lba, cnt));
				inputV[inIdx].nr_cnt -= cnt;
				inputV[inIdx].totalSeq++;
				cnt = 0;
				break;
			}
		}
	}
	printf("--sequence ordering done--\n");

	fseek(lbaFp, 0, SEEK_SET);
	ULL matchLbaToIdx;
	while(!feof(lbaFp)) {
		fscanf(lbaFp, "%lld %lld %lld", &inputOp, &inputLBA, &inputSize);
		if(inputOp == 0) wInfo(dacFp, 0, inputLBA, inputSize);
		else if(inputOp == 1) {
			wInfo(dacFp, 1, inputLBA, inputSize);
			for(ULL index=0; index < inputSize; index++) {
				matchLbaToIdx = matchIdx[inputLBA + index];
				infoLBA& c = inputV[matchLbaToIdx];

				ULL idx = c.seq[c.curSeq].first;
				if( (--c.seq[c.curSeq].second) ==0)
					c.curSeq++;

				for(int i=0; i < (PAGE_SIZE / 1024); i++) {
					if(rBox.getNumber() < pDiff) {
						dBox.cLeftShift(idx, i, (PAGE_SIZE/4));
					}
				}
				fwrite(dBox.buf[idx], 1, PAGE_SIZE, dacFp);
			}
		}

	}

	printf("--File write done--\n");



	fclose(lbaFp);
	fclose(dacFp);

	return 0;
}


void wInfo(FILE* fp,ULL op, ULL lpa, ULL size) {
	fwrite(&op, 8,1, fp);
	fwrite(&lpa,8,1, fp);
	fwrite(&size,8,1, fp);
}
