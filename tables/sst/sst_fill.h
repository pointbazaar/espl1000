#ifndef SST_FILL_H
#define SST_FILL_H

//utilities to help fill the SST

struct Namespace;
struct Method;

struct SST;
struct ST;

void sst_fill(struct ST* st, struct SST* sst, struct Namespace* ns);

struct Type* method_to_subrtype(struct Method* m);

#endif
