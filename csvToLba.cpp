#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* getfield(char* line, FILE* Ofp)
{
	const char* tok;
	const char* op;
	int i=0;
	long long offset, size,len, lba, nr_page;
	for (tok = strtok(line, ",");
			tok && *tok;
			tok = strtok(NULL, ",\n"), i++)
	{
		if(i==3) {
			op = tok;
		} else if(i==4) {
			offset = atoll(tok);
		} else if(i==5) {
			size = atoll(tok);
		}

	}
//		printf("size : %lld\n", size);

	lba = offset / 4096;
//	printf("offset : %lld, lba : %lld\n", offset, lba);
	len = offset + size;
	if(len % 4096 != 0) len = (len/4096) +1;
	else len = (len / 4096);

	nr_page = len - lba;

//	printf("len : %lld, lba : %lld, nr_page : %lld\n", len, lba, nr_page);

	if(strcmp(op,"Read")==0) {
		fprintf(Ofp, "0 %lld %lld\n", lba, nr_page);

	} else if(strcmp(op, "Write") ==0) {
		fprintf(Ofp, "1 %lld %lld\n", lba, nr_page);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	FILE* stream = fopen(argv[1], "r");
	FILE* Ofp = fopen(argv[2], "w");

	char line[1024];
	while (fgets(line, 1024, stream))
	{
		getfield(line, Ofp);
	}
}
