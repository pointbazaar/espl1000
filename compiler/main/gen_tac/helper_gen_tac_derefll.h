#ifndef HELPER_GEN_TAC_DEREFLL_H
#define HELPER_GEN_TAC_DEREFLL_H

struct TACBuffer;
struct DerefLL;
struct Type;
struct Ctx;

void tac_derefll_single(struct TACBuffer* buffer, struct DerefLL* dll, struct Type* prev_type, struct Ctx* ctx);

#endif
