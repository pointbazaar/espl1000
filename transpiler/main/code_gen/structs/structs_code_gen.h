#ifndef STRUCTS_CODE_GEN_H
#define STRUCTS_CODE_GEN_H

struct Namespace;
struct Ctx;


void gen_struct_subrs(struct Namespace* ns, struct Ctx* ctx);
void gen_struct_subr_signatures(struct Namespace* ns, struct Ctx* ctx);

#endif
