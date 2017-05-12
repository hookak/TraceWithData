#define PAGE_SIZE 4096
#define NR_PAGE 10000

class pInfo {
	public:

	unsigned char data[PAGE_SIZE];
	unsigned long long nr_update;
	unsigned long long nr_diff;

	pInfo();
};

class pageTable {
	public:
	pInfo* table[NR_PAGE];
	unsigned long long data_ch[256];
	unsigned long long total_write_reqs;
	unsigned long long total_update_reqs;
	unsigned long long total_page_diff;

	pageTable();
	int diff(unsigned char*, unsigned char*);
	void update(int ,unsigned char*);
	void analysis();
	double calEnt();
	
};
