#include "page.h"
#include <cstdio>
#include <cstring>
#include <cmath>

pInfo::pInfo() {
	nr_update =0;
	nr_diff = 0;
}

pageTable::pageTable() {
	for(int i=0; i<NR_PAGE; i++) {
		table[i] = new pInfo;
	}

	for(int i=0; i<256; i++) data_ch[i] = 0;
	total_write_reqs = 0;
	total_update_reqs = 0;
	total_page_diff = 0;
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

void pageTable::update(int offset, unsigned char* input) {
	/*offset unit is 4kb */
	int idx = offset;
	int nr_diff;
	pInfo* cur = table[idx];
	if(cur->nr_update != 0) {
		nr_diff = diff(input, cur->data);
		cur->nr_diff += nr_diff;
		total_page_diff += nr_diff;
		total_update_reqs++;
	}
		
	cur->nr_update++;
	memcpy(cur->data, input, PAGE_SIZE);
	for(int i=0; i< PAGE_SIZE; i++) {
		int idx = input[i];
		data_ch[idx]++;
	}

	total_write_reqs++;
}

double pageTable::calEnt() {
	double ent = 0;
	for(int i=0; i<256; i++) {
		double temp = (double)data_ch[i] / ((PAGE_SIZE/8)* total_write_reqs) ;
		if(data_ch[i] ==0)
			ent -= 0;
		else
			ent -= temp * log2(temp);

	}

	return ent;
}

void pageTable::analysis() {
	double ent = calEnt();
	double dupRate = (double)total_update_reqs / total_write_reqs;
	double pageDiff = (double)total_page_diff / (total_update_reqs * 4);

	printf("%5.2lf %5.2lf ????? %5.2lf\n", ent, dupRate, pageDiff);
}
