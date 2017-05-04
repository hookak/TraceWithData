#define PAGE_SIZE 4096
#define NR_PAGE 10000

#include <random>
#include <ctime>
#include <functional>
using namespace std;

class random_box {
	public:
	mt19937 engine((unsigned int)time(NULL));
	uniform_int_distribution<int>* bit;
	uniform_real_distribution<double>* U_R_D;
	normal_distribution<double>* N_R_D;
	uniform_int_distribution<int>* U_I_D;
	normal_distribution<double>* N_I_D;

	double ent;
	random_box(double);
	int getNumber(void);
	double getEntropy(int);
	int getIndex(int);


};

class data_box {
	public:
	unsigned char* buf[NR_PAGE];
	int dup_idx[NR_PAGE];

	data_box(double);
	
	int genData( random_box&, int mode);
	void init_dupIdx(void);
	void cLeftShift(int , int);
	void cRightShift(int, int);
};

double Rounding(double, int);

