#include "ent.h"
#include <cstring>
#include <cmath>

double Rounding(double x, int digit) {
	return (floor((x)*pow(float(10), digit) + 0.5f) / pow(float(10), digit) );
}

random_box::random_box(double e) {
	ent = e;
	bit = new uniform_int_distribution<int>(0,9999);
	U_R_D = new uniform_real_distribution<double>(ent-0.035, ent+0.035);
	N_R_D = new normal_distribution<double>(ent, 0.035);
	U_I_D = new uniform_int_distribution<int>(0, NR_PAGE-1);
	N_I_D = new normal_distribution<double>(NR_PAGE/2, NR_PAGE/100);
	
}

int random_box::number(void) {
	return (*bit)(engine);
}

/* 0 : uniform,  1 : normal */
double random_box::entropy(int mode) {
	double r;
	if(0 == mode)
		r = (*U_R_D)(engine);
	else if(1 == mode)
		r = (*N_R_D)(engine);

	return r;
}

int random_box::index(int mode) {
	int idx;
	if( 0 == mode)
		idx = (*U_I_D)(engine);
	else if( 1 == mode)
		idx = (*N_I_D)(engine);

	return idx;
}

data_box::data_box(double e) {
	rBox = new random_box(e);

	for(int i=0; i< NR_PAGE; i++)
		buf[i] = (unsigned char*)malloc(sizeof(unsigned char)*PAGE_SIZE);

	init_dupIdx();
}

void data_box::init_dupIdx(void) {
	for(int i=0; i< NR_PAGE; i++) dup_idx[i] = i;
	for(int i=0; i< NR_PAGE; i++) {
		int dest = rand() % NR_PAGE;
		int temp = dup_idx[i];
		dup_idx[i] = dup_idx[dest];
		dup_idx[dest] = temp;
	}
}

void data_box::cLeftShift(int idx, int len) {
	unsigned char* s = buf[idx];
	unsigned char temp = s[len - 1];
	for(int i=0; i < len-1; i++)
		s[i] = s[i+1];
	s[len-1] = temp;
}

void data_box::cRightShift(int idx, int len) {
	unsigned char* s = buf[idx];
	unsigned char temp = s[len - 1];
	for(int i= len-1; i >0; i--)
		s[i] = s[i-1];
	s[0] = temp;
}
