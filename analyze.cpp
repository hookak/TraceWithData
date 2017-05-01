#include <cstdio>
#include <cstring>
#include <map>
using namespace std;

class page {
	public:
	int nr_update;
//	char data[4096];
	page() { nr_update = 0;}
};

void analyze_write_req(int offset, int size);
void analyze_read_req(int offset, int size);
long long total_read_reqs, total_write_reqs;
map<int, page> pat;

int main(int argc, char* argv[]) {
	FILE* fp;
	int tmp;
	char tmps[64], ops[16];
	int sec,usec,offset,size;


	fp = fopen(argv[1], "r");

	while(!feof(fp)) {
		fscanf(fp, "%d,%d %d %d %d.%d %d %[^ ] %[^ ] %d + %d %[^\n]",\
				&tmp, &tmp, &tmp, &tmp, &sec, &usec, &tmp, \
				tmps, ops, &offset, &size, tmps);
	//	printf("%s %d + %d\n", ops,offset,size);

		if(ops[0] == 'W') {
			analyze_write_req(offset, size);
		}
		else if(ops[0] == 'R') {
			analyze_read_req(offset,size);
		}
	}


	map<int,page>::iterator it;
	for(it = pat.begin(); it != pat.end(); it++) {
		printf("%d %d\n", it->first, it->second.nr_update);
	}




	fclose(fp);

	return 0;
}

void analyze_write_req(int offset, int size) {
	int nr_page = size / 8;
	int addr = offset/8;
	page p;

	if(offset%8 !=0) printf("Not match 4kb align\n");

	for(int loop=0; loop < nr_page; loop++) {
		int lpa = addr + loop;
	//	printf("lpa : %d\n", lpa);
		if(pat.count(lpa)==0) { // is not an element of pat
			pat[lpa] = p;
		} else {
			p = pat[lpa];
			/* some operation for analyze data locality */
			p.nr_update++;
			pat[lpa] = p;
		}
		
	}
}

void analyze_read_req(int offset, int size) {
}
