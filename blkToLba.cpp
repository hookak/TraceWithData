#include <cstdio>

int main(int argc, char* argv[]) {
	FILE* fp;
	FILE* ofp;
        char tmps[64];
        char ops[16];
        float tmpf;
        int tmp;
	unsigned long long offset, size;
	int sec, usec;

	fp = fopen(argv[1], "r");
	ofp = fopen(argv[2], "w");


	while(!feof(fp)) {

		fscanf(fp, "%d,%d %d %d %d.%d %d %[^ ] %[^ ] %lld + %lld %[^\n]", \
				&tmp, &tmp, &tmp, &tmp, &sec, &usec, &tmp, \
				tmps, ops, &offset, &size, tmps);


		if(ops[0] == 'W') {
			offset = offset /8;
			size = size/8;
			fprintf(ofp, "1 %lld %lld\n", offset, size);
		}else if(ops[0] == 'R') {
			offset = offset / 8;
			size = size /8;
			fprintf(ofp, "0 %lld %lld\n", offset, size);

		}
		 

	}

}
