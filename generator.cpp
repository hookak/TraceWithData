#include "box.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

void wInfo(FILE*,ULL op, ULL lpa, ULL size);


class infoLBA {
	public:
	ULL lba, w_cnt,r_cnt, curSeq, totalSeq;
	ULL compLba;
	vector<pair<ULL,ULL>> seq;

	infoLBA(ULL l) {
		lba = l;
		w_cnt=0;
		r_cnt=0;
		curSeq=0;
		totalSeq = 0;
		compLba = -1;
	}
};


bool comp(const pair<ULL,ULL> a, const pair<ULL,ULL> b) { return a.second > b.second;}
bool comp1(const infoLBA a, infoLBA b) {return a.w_cnt > b.w_cnt;}

vector<pair<ULL,ULL>> genV;
vector<infoLBA> tempV;
vector<infoLBA> inputV;
vector<ULL> matchIdx;
vector<ULL> sortedIdx;

int main(int argc, char* argv[]) {
	//FILE* dataFp, *traceFp;
	FILE* dacFp;
	FILE* lbaFp;
	double entropy, pageDiff, lbaRange;
	int entDist, pageDist, RW;
	int  pDiff;
	char pDist[10];
	char rwMode[10];
	ULL maxLBA=0, total_4K_write_reqs=0;
	ULL nr_page;


//	dataFp = fopen("a.dat", "w");
//	traceFp = fopen("trace.in", "w");
	dacFp = fopen(argv[1], "w");
	lbaFp = fopen(argv[2], "r");

	//	if( (NULL == dataFp || NULL == traceFp )|| NULL == dacFp) return -1;
	if(argc != 8) {
		printf("Usage : %s [Output File] [LBA File] [Entropy] [Page Distribution] [Page Difference] [LBA Range] [RW Mode]\n", argv[0]);
		return -1;
	}
	if(NULL == dacFp) {return -1;}
	if(NULL == lbaFp) {return -1;}

	entropy = Rounding(atof(argv[3]),2);
	pageDist = atoi(argv[4]);
	pageDiff = atof(argv[5]);
	lbaRange = atof(argv[6]);
	RW = atoi(argv[7]);

	entDist = 1; 
	if(pageDist == 0) strcpy(pDist, "UNIFORM");
	else if(pageDist == 1) strcpy(pDist, "NORMAL");

	if(RW == 0) strcpy(rwMode, "RW");
	else if(RW == 1) strcpy(rwMode, "W");
	pDiff = pageDiff*10000;

	printf("Entropy : %.3f, Page Distribution : %s, Page Difference : %.3f, LBA Range : %.3f, MODE : %s\n" \
			, entropy,pDist, pageDiff, lbaRange, rwMode);


	/* lba.in File read */
	ULL inputLBA, inputSize, inputOp;
	ULL pageIdx, idx;

	while(!feof(lbaFp)) {
		fscanf(lbaFp, "%lld %lld %lld", &inputOp, &inputLBA, &inputSize);
		for(ULL i=0; i< inputSize; i++) {
			if(maxLBA < (inputLBA + i)) maxLBA = (inputLBA+i);
		}
		if(inputOp == 1) total_4K_write_reqs += inputSize; //write Request
		//printf("%lld %lld %lld %lld\n", inputOp, inputLBA, inputSize, total_4K_write_reqs);
	}
	fseek(lbaFp, 0, SEEK_SET);

	
	printf("MaxLBA : %lld [%.2f G], Total 4K Write req :  %lld [%.2f G]\n" \
			, maxLBA, (double)maxLBA*4096/1024/1024/1024, total_4K_write_reqs, (double)total_4K_write_reqs*4096/1024/1024/1024);
	
	for(ULL i=0; i<= maxLBA ; i++) {
		tempV.push_back(i);
		matchIdx.push_back(0);
		sortedIdx.push_back(0);
	}

	while(!feof(lbaFp)) {
		fscanf(lbaFp, "%lld %lld %lld",&inputOp, &inputLBA, &inputSize);
		if(inputOp==1) {
			for(ULL i=0; i< inputSize; i++)
				tempV[inputLBA+i].w_cnt++;
		} else if(inputOp == 0) {
			for(ULL i=0; i< inputSize; i++)
				tempV[inputLBA+i].r_cnt++;
		}
	}


	/*LBA compression */
	ULL compMax=0;
	for(ULL i=0; i <= maxLBA; i++) {
		if(tempV[i].w_cnt !=0 || tempV[i].r_cnt != 0) {
			inputV.push_back(i);
			inputV[compMax].compLba = compMax;
			inputV[compMax].w_cnt = tempV[i].w_cnt;
			inputV[compMax].r_cnt = tempV[i].r_cnt;
			compMax++;
		}
	}
	tempV.clear();

	nr_page = (compMax * lbaRange);

	printf("CompLBA : %lld [%.2f G],   GeneratedLBA : %lld [%.2f G]\n"\
			, compMax-1, ((double)(compMax-1)*4096 / 1024 / 1024 / 1024), nr_page, (double)nr_page*4096/1024/1024/1024);

	random_box rBox(entropy, nr_page);
	data_box dBox(entropy, nr_page);
	dBox.genData(rBox, entDist);

	printf("Data Initialized\n");


	for(ULL i=0; i< dBox.NR_PAGE; i++) genV.push_back(make_pair(i,0));


	/* make lba sequence */
	for(ULL i=0; i < total_4K_write_reqs; i++) {
		idx = rBox.getIndex(pageDist);
		pageIdx = dBox.dup_idx[idx];
		genV[pageIdx].second++;
	}

	//int middle = dBox.dup_idx[NR_PAGE/2];
	/*
	   for(int i=0; i< NR_PAGE; i++) {
	   printf("%lld %lld\n", genV[i].first, genV[i].second);
	   }
	 */
	/* matchIdx */
	for(ULL i=0; i < inputV.size(); i++) {
		ULL lba = inputV[i].lba;
		matchIdx[lba] = i;
	}

	printf("MatchIdx done\n");

	sort(inputV.begin(), inputV.end(), comp1);
	sort(genV.begin(), genV.end(), comp);

	for(ULL i=0; i < inputV.size(); i++) {
		ULL lba = inputV[i].lba;
		sortedIdx[lba] = i;
	}
	


	/* sequence ordering */
	ULL inIdx=0;
	for(ULL i=0; i < dBox.NR_PAGE; i++) {
		ULL lba = genV[i].first;
		ULL& cnt = genV[i].second;
		while(1) {
	//		printf("inIdx : %lld\n", inIdx);
			ULL w_cnt = inputV[inIdx].w_cnt;
			if( w_cnt  <= cnt) {

				inputV[inIdx].seq.push_back(make_pair(lba, w_cnt));
				inputV[inIdx].w_cnt = 0;
				inputV[inIdx].totalSeq++;
				cnt -= w_cnt;
				//	printf("lba : %lld, Cnt : %lld\n",lba, cnt);
				inIdx++;
				if(cnt ==0) break;
			} else {
				inputV[inIdx].seq.push_back(make_pair(lba, cnt));
				inputV[inIdx].w_cnt -= cnt;
				inputV[inIdx].totalSeq++;
				cnt = 0;
				break;
			}
		}
	}
	printf("--sequence ordering done--\n");

	fseek(lbaFp, 0, SEEK_SET);
	ULL matchLbaToIdx;
	int ret1;
	while(!feof(lbaFp)) {
		ret1 = fscanf(lbaFp, "%lld %lld %lld", &inputOp, &inputLBA, &inputSize);
		if(ret1 !=3) break;

		ULL sortedIndex = sortedIdx[inputLBA];
		ULL compLBA = inputV[sortedIndex].compLba;
		if(inputOp == 0 && RW == 0) wInfo(dacFp, 0,compLBA , inputSize);
		if(inputOp == 1) {
			wInfo(dacFp, 1, compLBA, inputSize);
			for(ULL index=0; index < inputSize; index++) {
				matchLbaToIdx = sortedIdx[inputLBA + index];
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
