#ifndef SMALLDRAGON_TOPLEVEL_TACBUFFER_H
#define SMALLDRAGON_TOPLEVEL_TACBUFFER_H

#include <string.h>
#include <stdbool.h>

struct TACBuffer;
struct TAC;

//TACBuffer functions
struct TACBuffer* tacbuffer_ctor();
void tacbuffer_dtor(struct TACBuffer* buffer);
void tacbuffer_print(struct TACBuffer* buffer);
char* tacbuffer_tostring(struct TACBuffer* buffer, bool graphviz);
void tacbuffer_append(struct TACBuffer *buffer, struct TAC *node);

void tacbuffer_set(struct TACBuffer* buffer, int index, struct TAC* node);
struct TAC* tacbuffer_get(struct TACBuffer* buffer, int index);
struct TAC* tacbuffer_get_last(struct TACBuffer* buffer);

char* tacbuffer_last_dest(struct TACBuffer* buffer);
size_t tacbuffer_count(struct TACBuffer* buffer);

size_t tacbuffer_indexof(struct TACBuffer* buffer, struct TAC* tac);
#endif
