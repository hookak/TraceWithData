
#define BUFSIZE 4096
#define NR_PAGE 10000

int genData(char* fileName, const double entropy);
double calEnt(char* fileName);
//double log2(double) { return log(x)/log(2); }
