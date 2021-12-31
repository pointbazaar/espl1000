#ifndef SMALLDRAGON_TOPLEVEL_TACBUFFER_H
#define SMALLDRAGON_TOPLEVEL_TACBUFFER_H

#include <string.h>
#include <stdbool.h>

struct TACBuffer{
    size_t cap;
    size_t count;
    struct TAC** buffer;
};

//TACBuffer functions
struct TACBuffer* tacbuffer_ctor();
void tacbuffer_dtor(struct TACBuffer* buffer);
void tacbuffer_print(struct TACBuffer* buffer);
char* tacbuffer_tostring(struct TACBuffer* buffer, bool graphviz);
void tacbuffer_append(struct TACBuffer* buffer, struct TAC* node);

uint32_t tacbuffer_indexof(struct TACBuffer* buffer, struct TAC* tac);

#endif
