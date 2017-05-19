#include "page.h"
#include <cstdio>
#include <cstring>
using namespace std;
typedef unsigned long long ULL;
void analyze_write_req(pageTable*,ULL offset, ULL size, unsigned char*);
void analyze_read_req(int offset, int size);
void rInfo(FILE* fp,ULL* op, ULL* lpa, ULL* size); 


int main(int argc, char* argv[]) {
	FILE* fp;
	int tmp,n;
	char tmps[64], ops[16];
	unsigned long long op, lpa_start, size;
	unsigned char data[PAGE_SIZE];
	pageTable* pTable;
	pTable = new pageTable;


	if(argc != 2) {
		printf("Usage : %s [Dac format file]\n", argv[0]);
		return -1;
	}


	fp = fopen(argv[1], "r");

	while(!feof(fp)) {
		/*
			fscanf(fp, "%d,%d %d %d %d.%d %d %[^ ] %[^ ] %d + %d %[^\n]",\
				&tmp, &tmp, &tmp, &tmp, &sec, &usec, &tmp, tmps, ops, &offset, &size, tmps);
		*/

		rInfo(fp, &op, &lpa_start, &size);
		fread(data, 1, PAGE_SIZE, fp);

	//	printf("%lld %lld %lld\n", op,lpa_start,size);
	//	printf("%s %d + %d\n", ops,offset,size);

		if(op == 1) {

			analyze_write_req(pTable, lpa_start , size, data);
		}
		else if(op == 0) {
			analyze_read_req(lpa_start,size);
		}
	}

	pTable->analysis();


	fclose(fp);

	return 0;
}

void analyze_write_req(pageTable* pat, ULL offset, ULL size, unsigned char* data) {
//	printf("analyze_write_req\n");
	ULL nr_page = size / 8;
	ULL addr = offset/8;

	if(offset%8 !=0) printf("Not match 4kb align\n");

	for(ULL loop=0; loop < nr_page; loop++) {
		ULL lpa = addr + loop;
	//	pat->update(lpa, data);
//		printf("%lld %lld\n", lpa, pat->table[lpa]->nr_update);
		int nr_diff;
		pInfo* cur = pat->table[lpa];

		if(cur->nr_update !=0) {
			nr_diff = pat->diff(data, cur->data);
			cur->nr_diff += nr_diff;
			pat->total_page_diff += nr_diff;
			pat->total_update_reqs++;
		}

		cur->nr_update++;
		
		memcpy(cur->data, data, PAGE_SIZE);

		
		unsigned char idx;
//		printf("%c\n", data[3]);
		
		for(int i=0; i<PAGE_SIZE; i++) {
			idx = data[i];
			pat->data_ch[(unsigned int)idx]++;
		}
		
		
		pat->total_write_reqs++;
		

		
	}

}

void analyze_read_req(int offset, int size) {
}

void rInfo(FILE* fp, ULL* op, ULL* lpa, ULL* size) {
	fread(op, 8,1, fp);
	fread(lpa, 8,1, fp);
	fread(size, 8,1,fp);
}
