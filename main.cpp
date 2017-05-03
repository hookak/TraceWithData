#include <iostream>
#include <map>
#include <cstring>
#include <cmath>
#include <string>
#include <iomanip>
#include <map>
#include "ent.h"


void init_dupIdx(int*);
void cLeftShift(unsigned char *,int);
void cRightShift(unsigned char *, int);

int main(int argc, char* argv[])
{

	double entropy;
	int dist; // 0: uniform, 1: normal
	unsigned char* buf[NR_PAGE];
	double bufEnt[NR_PAGE];
	int dup_idx[NR_PAGE];
	char dataFile[20] = "a.dat";
	char traceFile[20] = "trace.in";
	FILE *fp1, *fp2;

	if((fp1 = fopen(dataFile, "w"))==NULL)
		return -1;
	if((fp2 = fopen(traceFile, "w"))==NULL)
		return -1;


	for(int i=0; i<NR_PAGE; i++) 
		buf[i] = (unsigned char*)malloc(sizeof(unsigned char)*PAGE_SIZE);

	
	if(argc !=5 ) {
		printf("Usage :%s Entropy Duplication Page_Distribution Page_Difference\n", argv[0]);
		return -1;
	}
	entropy = strtod(argv[1],0);
	dist = 1;


	srand(time(NULL));
	random_device rd;
	mt19937 generator(rd());  //random engine is mt19937
	uniform_int_distribution<> bit_dist(0,10000);
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
		if(genData(buf[i], cor,bit_dist,generator) !=0) return -1;
		/* caclulate file entropy */
		bufEnt[i]=calEnt(buf[i], bufEnt+i);

		ent_hist[bufEnt[i]]++;
	}

	//calculate entropy total page
	long long data_ch[256] ={0};
	double dataEnt =0.0;
	for(int i=0; i<NR_PAGE; i++) {
		for(int j=0; j< PAGE_SIZE/8; j++) {
			unsigned char idx = buf[i][j];
		//	cout << i << " " << j << " : " << (int)idx << endl;
			data_ch[(int)idx]++;
		}
	}

	cout << "PAGE_SIZE : " << PAGE_SIZE << endl;
	for(int i=0; i<256; i++) {
		double temp = (double)data_ch[i]/((PAGE_SIZE*NR_PAGE)/8);
	//	cout << i << " temp : " << temp << endl;
		if(data_ch[i]==0)
			dataEnt -= 0;
		else {

			dataEnt -= temp*log2(temp);
			//cout << i << " temp*log2(temp) = " << -temp*log2(temp) << endl;
		}
	}
	dataEnt = Rounding((dataEnt/8),2);

	cout << NR_PAGE << " pages Entropy : " << dataEnt << endl;

	//file write
	for(int i=0; i< NR_PAGE; i++)
		fwrite(buf[i],1,PAGE_SIZE, fp1);

	//duplication rate

	double dupRate = strtod(argv[2],0);
	dupRate = Rounding(dupRate,2);
	int mode = atoi(argv[4]);
	double per_dup = dupRate/(1-dupRate)*NR_PAGE;
	int nr_dup =(int)per_dup;  //num of dup pages
	map<int,int> file_hist;
	int idx;
	int noiseStrength = 1; // 0: 0%, 1: 25%, 2: 50%;
	double noiseFrequency = 0.5; // 0 < noiseFrequency < 1
	unsigned char* source;

	uniform_int_distribution<int> u_i_d(0, nr_dup);
	normal_distribution<double> n_d_d((nr_dup/2),10);


	// initial and suffle
	init_dupIdx(dup_idx);

	cout <<  "dupRate : " << argv[3] << " more " << nr_dup << " pages " << endl;
	for(int i=0; i<nr_dup; i++) {
		if(mode==0)idx = u_i_d(generator);
		else if(mode==1) idx = n_d_d(generator);
		//cout << idx << "  " << dup_idx[idx] <<  endl;

		idx = dup_idx[idx];
		if(idx <0) idx = 0;
		if(idx >nr_dup) idx = nr_dup;

		int r = rand()%100;


		if(r < noiseFrequency*100) {
			for(int j=0; j< noiseStrength; j++) {
				int shuffle = rand()%4;
				source = buf[idx]+ shuffle*(PAGE_SIZE/4);

				cLeftShift(source, PAGE_SIZE/4);
				/* page write */
				fwrite(buf[idx], 1, PAGE_SIZE, fp);
				cRightShift(source, PAGE_SIZE/4);

			}
		} else {
			fwrite(buf[idx],1, PAGE_SIZE, fp);
		}

		file_hist[idx]++;
	}


	//display

	/*
	if(dist==0) cout << "\t-UNIFORM DISTRIBUTION-" << endl;
	else if(dist==1) cout << "\t-NORMAL DISTRIBUTION-" << endl;
	cout << "\n Generate entropy distribution around " << entropy << "\n";

	map<double,int>::iterator p;
	for(p=num_hist.begin(); p != num_hist.end(); p++) {
		std::cout << std::fixed << std::setprecision(2) << std::setw(2)
			<< p->first << ' ' << string(p->second/50, '*') << '\n';
	}
	
	cout << "\n Calculate entropy distribution around " << dataEnt << "\n";
	for(p = ent_hist.begin(); p != ent_hist.end(); p++) {
		std::cout << std::fixed << std::setprecision(2) << std::setw(3)
			<< p->first << ' ' << string(p->second/50, '*') << '\n';
	}
	*/
	fclose(fp);

	return 0;
}


double Rounding(double x, int digit) {
	return ( floor((x)*pow(float(10), digit) + 0.5f) / pow(float(10),digit) );
}

void init_dupIdx(int* dup_idx)	
{
	for(int i=0; i<NR_PAGE;i++) dup_idx[i]=i; 
	for(int i=0; i<NR_PAGE; i++) {
		int dest = rand() % NR_PAGE;
		int temp = dup_idx[i];
		dup_idx[i] = dup_idx[dest];
		dup_idx[dest] = temp;
	}
}


void cLeftShift(unsigned char* s, int len) 
{
	char temp = s[0];
	for(int i=0; i< len-1; i++)
		s[i] = s[i+1];
	s[len-1] = temp;
}

void cRightShift(unsigned char* s, int len)
{
	char temp = s[len-1];
	for(int i=len-1; i>0; i--) 
		s[i] = s[i-1];
	s[0] = temp;
}
