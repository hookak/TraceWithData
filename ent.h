#define PAGE_SIZE 4096
#define NR_PAGE 1000

#include <random>
using namespace std;

int genData(unsigned char* buf, const double entropy, uniform_int_distribution<>&, mt19937&);
double calEnt(unsigned char* buf, double* bufEnt);
double Rounding(double, int);

