#include "page.h"
#include <cstdio>
#include <cstring>
#include <cmath>

pInfo::pInfo() {
	data = new unsigned char [PAGE_SIZE];
	nr_update =0;
	nr_diff = 0;
}

pageTable::pageTable() {
	table = new pInfo* [NR_PAGE];
	
	for(int i=0; i<NR_PAGE; i++) {
		table[i] = new pInfo;

//		printf("table[%d] %lld %lld\n", i, table[i]->nr_update, table[i]->nr_diff);
	}


	for(int i=0; i<256; i++) data_ch[i] = 0;
	total_write_reqs = 0;
	total_update_reqs = 0;
	total_page_diff = 0;
	sumP = 0.0;
}


int pageTable::diff(unsigned char* source, unsigned char* dest) {
	unsigned char* s,*d;
	int nr_subpage = 4;
	int nr_diff = 0;
	for(int i=0; i< nr_subpage; i++) {
		s = source + (i * 1024);
		d = dest + (i * 1024);
		for(int j=0; j< (PAGE_SIZE/nr_subpage); j++) {
			if( (s[j] ^ d[j]) != (unsigned char)0x00) {
				nr_diff++;
				break;
			}
		}
	}

	return nr_diff;
}

void pageTable::update(unsigned long long offset, unsigned char* input) {
	/*offset unit is 4kb */
	int nr_diff;

	
	if(offset < 0 || offset >= NR_PAGE) {
		printf("offset error\n");
		return;
	}
	pInfo* cur = this->table[offset];
	printf("table[%lld]->nr_update %lld, %lld, Total WREQ : %lld\n", offset, cur->nr_update, cur->nr_diff, total_write_reqs);

	if(cur->nr_update != 0) {
		printf("in IF\n");
		nr_diff = diff(input, cur->data);
		cur->nr_diff += nr_diff;
		total_page_diff += nr_diff;
		total_update_reqs++;
	}
		
	cur->nr_update++;
	memcpy(cur->data, input, PAGE_SIZE);
	for(int i=0; i< PAGE_SIZE; i++) {
		char ch= input[i];
		data_ch[(int)ch]++;
	}

	total_write_reqs++;
}

double pageTable::calEnt() {
	double ent = 0;
	for(int i=0; i<256; i++) {
		double temp = (double)data_ch[i] / ((PAGE_SIZE)* total_write_reqs) ;
		if(data_ch[i] ==0) {
			ent -= 0;
			sumP +=0;
		}
		else {
			ent -= temp * log2(temp);
			sumP += temp;
		}

	}

	return ent;
}

void pageTable::analysis() {
	double ent = calEnt();
//	printf("%lld %lld\n", total_update_reqs, total_page_diff);
	double dupRate = (double)total_update_reqs / total_write_reqs;
	double pageDiff = (double)total_page_diff / (total_update_reqs * 4);

	printf("%5.2lf %5.2lf  %5.2lf\n", ent/8, dupRate, pageDiff);
}
