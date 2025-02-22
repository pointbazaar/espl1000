#ifndef SST_FILL_H
#define SST_FILL_H

//utilities to help fill the SST

struct Namespace;
struct Method;
struct MethodDecl;

struct SST;
struct ST;

void sst_fill(struct ST* st, struct SST* sst, struct Namespace* ns);

struct Type* method_to_type(struct Method* m);
// @returns NULL on error
struct SubrType* method_decl_to_subrtype(struct MethodDecl* mdecl);
struct Type* method_decl_to_type(struct MethodDecl* mdecl);
#endif
