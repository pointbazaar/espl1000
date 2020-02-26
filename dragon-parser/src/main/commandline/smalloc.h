#ifndef SMALLOC
#define SMALLOC

//safe malloc. either returns pointer != NULL, or does exit(1)
void* smalloc(int size);

#endif