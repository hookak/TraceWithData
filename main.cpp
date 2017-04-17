#include "ent.h"
#include <iostream>
#include <random>
#include <map>
#include <cstring>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std;

double Rounding(double, int);

int main(int argc, char* argv[])
{

	char fileName[30] = "gen.dat";
	double entropy;
	double ret;
	int dist; // 0: uniform, 1: normal
	
	if(argc !=3) {
		printf("Usage : %s entropy P.distribution\n", argv[0]);
		return -1;
	}
	entropy = strtod(argv[1],0);
	dist 	= atoi(argv[2]);

	
	srand(time(NULL));
	random_device rd;
	mt19937 generator(rd());  //random engine is mt19937
	uniform_real_distribution<> uniform_dist(entropy-0.035, entropy+0.035);
	normal_distribution<> normal_dist(entropy, 0.035);

	map<double, int> num_hist;
	map<double, int> ent_hist;

	double num;
	for(int i=0; i< NR_PAGE; i++) {
		if(dist==0) num = uniform_dist(generator);
		else if(dist==1) num = normal_dist(generator);

		if(num > 1) num=1;
		else if(num <0) num = 0;
		double r = Rounding(num, 2); //rounding
		num_hist[r]++;
		double cor = r > 0.98 ? r : r+0.02;


		/* generate file */
		if(genData(fileName,cor) !=0) return -1;
		/* caclulate file entropy */
		ret = calEnt(fileName)/8;
		ret = Rounding(ret,2);
		ent_hist[ret]++;

	//	cout << "Input : " << r << " ,Output : " << ret << "\n";
	}

	if(dist==0) cout << "\t-UNIFORM DISTRIBUTION-" << endl;
	else if(dist==1) cout << "\t-NORMAL DISTRIBUTION-" << endl;
	cout << "\n Generate entropy distribution around " << entropy << "\n";
	for(auto p : num_hist) {
		std::cout << std::fixed << std::setprecision(2) << std::setw(2)
			<< p.first << ' ' << string(p.second/50, '*') << '\n';
	}
	
	cout << "\n Calculate entropy distribution around " << entropy << "\n";
	for(auto p : ent_hist) {
		std::cout << std::fixed << std::setprecision(2) << std::setw(3)
			<< p.first << ' ' << string(p.second/50, '*') << '\n';
	}

	return 0;
}


double Rounding(double x, int digit) {
	return ( floor((x)*pow(float(10), digit) + 0.5f) / pow(float(10),digit) );
}



