#include <stdlib.h>
#include <stdio.h>

#include "ast/ast.h"

#include "tacbuffer.h"
#include "tac.h"

void tacbuffer_append(struct TACBuffer *buffer, struct TAC *node, bool set_index) {

    if(buffer->count >= buffer->cap)
        buffer->buffer = realloc(buffer->buffer, sizeof(struct TAC*) * (buffer->cap *= 2));

    buffer->buffer[buffer->count] = node;

    if(set_index)
        node->index = buffer->count;

    buffer->count += 1;
}

struct TACBuffer* tacbuffer_ctor(){

    struct TACBuffer* buffer = make(TACBuffer);

    *buffer = (struct TACBuffer){
            .buffer = malloc(sizeof(struct TAC*)*10),
            .count = 0,
            .cap = 10
    };
    return buffer;
}

void tacbuffer_dtor(struct TACBuffer* buffer){
    for(size_t i = 0; i < buffer->count; i++){
        free(buffer->buffer[i]);
    }
    free(buffer->buffer);
    free(buffer);
}

void tacbuffer_print(struct TACBuffer* buffer){
    char* s = tacbuffer_tostring(buffer, false);
    printf("%s",s);
    free(s);
}
char* tacbuffer_tostring(struct TACBuffer* buffer, bool graphviz){
    char* res = malloc(sizeof(char)*(buffer->count*100));
    strcpy(res, "");

    for(size_t k = 0; k < buffer->count; k++){
        char* s = tac_tostring(buffer->buffer[k]);
        strcat(res, s);

        strcat(res, (graphviz)?"\\l":"\n");

        free(s);
    }

    return res;
}

