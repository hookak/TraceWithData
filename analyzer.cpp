#include "page.h"
#include <cstdio>
#include <cstring>
using namespace std;

void analyze_write_req(pageTable&,int offset, int size, unsigned char*);
void analyze_read_req(int offset, int size);


int main(int argc, char* argv[]) {
	FILE* fp;
	int tmp,n;
	char tmps[64], ops[16];
	unsigned long long op, lpa_start, size;
	unsigned char data[PAGE_SIZE];
	pageTable pTable;


	if(argc != 2) {
		printf("Usage : %s [Dac format file]\n");
		return -1;
	}


	fp = fopen(argv[1], "r");

	while(!feof(fp)) {
		/*
			fscanf(fp, "%d,%d %d %d %d.%d %d %[^ ] %[^ ] %d + %d %[^\n]",\
				&tmp, &tmp, &tmp, &tmp, &sec, &usec, &tmp, tmps, ops, &offset, &size, tmps);
		*/

		fscanf(fp, "%lld %lld %lld", &op, &lpa_start, &size);
		fread(data, 1, PAGE_SIZE, fp);

		printf("%lld %lld %lld\n", op,lpa_start,size);
	//	printf("%s %d + %d\n", ops,offset,size);

		if(op == 1) {

			analyze_write_req(pTable, lpa_start , size, data);
		}
		else if(op == 0) {
			analyze_read_req(lpa_start,size);
		}
	}

	pTable.analysis();


	fclose(fp);

	return 0;
}

void analyze_write_req(pageTable& pat, int offset, int size, unsigned char* data) {
	int nr_page = size / 8;
	int addr = offset/8;

	if(offset%8 !=0) printf("Not match 4kb align\n");

	for(int loop=0; loop < nr_page; loop++) {
		int lpa = addr + loop;
		pat.update(lpa, data);
	}

}

void analyze_read_req(int offset, int size) {
}
