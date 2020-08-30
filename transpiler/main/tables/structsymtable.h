#ifndef STRUCTSYMTABLE
#define STRUCTSYMTABLE

//TODO: finish the implementation 

struct STSTLine {
	
	char name[32];
	
};

struct STST {
	
	//STruct Symbol Table
	
	struct STSTLine** lines;
	
	unsigned int count;
	unsigned int capacity;
};

#endif
