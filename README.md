
Usage

1. generator 
	./generator [File] [Entropy] [duplication rate] [Page distribution] [Page Difference]

	[File]
		File name that you make
		
	[Entropy] <br />
		Entropy value is between [0,1]<br />

	[duplication rate] <br />
		rate is between [0,1] <br />

	[Page distribution] <br />
		support two distribution <br />
		0 : Uniform <br />
		1 : Normal <br />

	[Page Difference] <br />
		Difference is between [0,1] <br />
		

2. analyzor
	./analyzor [File]
	
	[File]
		File name that generator made
		

File Format

Uint64_t Request type   
	0 : Read
	1 : Write
	
Uint64_t lpa       
	Unit : 512B(Sector) 
	
Uint64_t size  
	Unit : 512B(Sector)
	
unsigned char data[PAGE_SIZE]
	PAGE_SIZE : 4K
	

 
