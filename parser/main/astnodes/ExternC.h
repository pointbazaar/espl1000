#ifndef SMALLDRAGON_TOPLEVEL_EXTERNC_H
#define SMALLDRAGON_TOPLEVEL_EXTERNC_H

struct ExternC;
struct TokenList;

struct ExternC* makeExternC(struct TokenList* tokens);

#endif //SMALLDRAGON_TOPLEVEL_EXTERNC_H
