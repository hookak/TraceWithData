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


		offset = offset /8;
		size = size/8;
		for(unsigned long long i=0; i< size; i++) {
			fprintf(ofp, "%lld\n", (offset + i));
		}

	}

}
