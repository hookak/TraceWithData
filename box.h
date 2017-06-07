#define PAGE_SIZE 4096

#include <random>
#include <ctime>
#include <functional>
#include <chrono>
using namespace std;
typedef unsigned long long ULL;


class random_box {
	public:
		uniform_int_distribution<int>* bit;
		uniform_real_distribution<double>* U_R_D;
		normal_distribution<double>* N_R_D;
		uniform_int_distribution<int>* U_I_D;
		normal_distribution<double>* N_I_D;

		double ent;
		ULL NR_PAGE;
		random_box(double, ULL);
		int getNumber(void);
		double getEntropy(int);
		int getIndex(int);


};

class data_box {
	public:
		unsigned char** buf;
		int* dup_idx;
		int NR_PAGE;

		data_box(double, ULL);

		int genData( random_box&, int mode);
		void init_dupIdx(void);
		void cLeftShift(int ,int,int);
		void cRightShift(int,int, int);
};

double Rounding(double, int);

