#ifndef SMALLDRAGON_EMIT_X86_H
#define SMALLDRAGON_EMIT_X86_H

void emit1(char* mnem, FILE* file);
void emit2(char* mnem, char* arg, FILE* file);
void emit_label(char* label, FILE* file);

#endif
