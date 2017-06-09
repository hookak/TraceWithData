#include "box.h"
#include <cstring>
#include <cmath>


mt19937 engine((unsigned int)time(NULL));

double Rounding(double x, int digit) {
	return (floor((x)*pow(float(10), digit) + 0.5f) / pow(float(10), digit) );
}

random_box::random_box(double e, ULL nr_page) {
	NR_PAGE = nr_page;
	ent = e;
	bit = new uniform_int_distribution<int>(0,9999);
	U_R_D = new uniform_real_distribution<double>(ent-0.035, ent+0.035);
	N_R_D = new normal_distribution<double>(ent, 0.035);
	U_I_D = new uniform_int_distribution<int>(0, NR_PAGE-1);
	N_I_D = new normal_distribution<double>(NR_PAGE/2, NR_PAGE/7);
	
}

int random_box::getNumber(void) {
	return (*bit)(engine);
}

/* 0 : uniform,  1 : normal */
double random_box::getEntropy(int mode) {
	double r;
	if(0 == mode)
		r = (*U_R_D)(engine);
	else if(1 == mode)
		r = (*N_R_D)(engine);

	if(r > 1) r =1;
	else if(r < 0) r = 0;

	return r;
}

int random_box::getIndex(int mode) {
	int idx;
	int nr_page = (int)NR_PAGE;
	if( 0 == mode)
		idx = (*U_I_D)(engine);
	else if( 1 == mode)
		idx = (*N_I_D)(engine);

	if(idx < 0) idx =0;
	else if(idx > nr_page-1) idx = nr_page-1;

	return idx;
}

/* px : Probability of 1 */
double px[] = { 1.0000, 0.9991, 0.9980, 0.9969, 0.9957, 0.9944, 0.9930, 0.9916, 0.9901, 0.9886, //0.01 to 0.09
		0.9879, 0.9854, 0.9837, 0.9820, 0.9803, 0.9785, 0.9766, 0.9747, 0.9728, 0.9709,
		0.9689, 0.9668, 0.9648, 0.9627, 0.9605, 0.9583, 0.9561, 0.9538, 0.9515, 0.9491,
		0.9468, 0.9443, 0.9419, 0.9394, 0.9368, 0.9342, 0.9315, 0.9289, 0,9262, 0.9234,
		0.9206, 0.9177, 0.9149, 0.9119, 0.9089, 0.9059, 0.9028, 0.8997, 0.8965, 0.8933,
		0.8899, 0.8866, 0.8832, 0.8798, 0.8763, 0.8727, 0.8691, 0.8654, 0.8616, 0.8578,
		0.8539, 0.8499, 0.8459, 0.8418, 0.8376, 0.8333, 0.8290, 0.8246, 0.8200, 0.8154,
		0.8107, 0.8059, 0.8010, 0.7959, 0.7908, 0.7855, 0.7801, 0.7745, 0.7689, 0.7630,
		0.7570, 0.7508, 0.7444, 0.7378, 0.7310, 0.7239, 0.7166, 0.7090, 0.7010, 0.6927,
		0.6840, 0.6747, 0.6650, 0.6545, 0.6432, 0.6309, 0.6172, 0.6016, 0.5831, 0.5588, 0.5000
		};


data_box::data_box(double e, ULL nr_page) {
	NR_PAGE = nr_page;
	buf = (unsigned char**)malloc(sizeof(unsigned char*)*NR_PAGE);
	dup_idx = (ULL*)malloc(sizeof(ULL)*NR_PAGE);

	for(ULL i=0; i< NR_PAGE; i++)
		buf[i] = (unsigned char*)malloc(sizeof(unsigned char)*PAGE_SIZE);

	init_dupIdx();
}

int data_box::genData(random_box& rBox, int mode) {


	double ent;
	int entInt, pInt, n;
	unsigned char* dataBuf;
	for(ULL i=0; i< NR_PAGE; i++) {
		//printf(" idx : %lld\n",i);
		ent = rBox.getEntropy(mode);
		ent = Rounding(ent,2);
		
		entInt = (int)(ent*100);
		pInt = (int)(px[entInt]*10000);
	//	if( rBox.getNumber()%2 == 0 ) pInt = 10000 - pInt;

		dataBuf = this->buf[i];
		memset(dataBuf, 0, PAGE_SIZE);
		for(int j=0; j< PAGE_SIZE; j++) {

			unsigned char uch = 0x01;
			for(int shift =0; shift<8; shift++) {
				n = rBox.getNumber();
				if(n < pInt) dataBuf[j] += uch;

				uch = uch<<1;
			}
		}
	}

	return 1;

}

void data_box::init_dupIdx(void) {
	for(ULL i=0; i< NR_PAGE; i++) dup_idx[i] = i;
	for(ULL i=0; i< NR_PAGE; i++) {
		int dest = rand() % NR_PAGE;
		int temp = dup_idx[i];
		dup_idx[i] = dup_idx[dest];
		dup_idx[dest] = temp;
	}

}

void data_box::cLeftShift(int idx, int subIdx, int len) {
	unsigned char* s = buf[idx] + subIdx * (PAGE_SIZE/4);
	unsigned char temp = s[len - 1];
	for(int i=0; i < len-1; i++)
		s[i] = s[i+1];
	s[len-1] = temp;
}

void data_box::cRightShift(int idx, int subIdx, int len) {
	unsigned char* s = buf[idx] + subIdx*(PAGE_SIZE/4);
	unsigned char temp = s[len - 1];
	for(int i= len-1; i >0; i--)
		s[i] = s[i-1];
	s[0] = temp;
}
