#include "ent.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
	char* fileName;
	double entropy;
	
	if(argc !=3) {
		printf("Usage : %s fileName entropy\n", argv[0]);
		return -1;
	}
	fileName = argv[1];
	entropy = strtod(argv[2],0);

	/* generate file */
	if(genData(fileName, entropy) !=0) return -1;
	/* caclulate file entropy */
	calEnt(fileName);

	return 0;
}




