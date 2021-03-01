#ifndef SERIALIZE_H
#define SERIALIZE_H


uint32_t deserialize_int(FILE* file);
void serialize_int(uint32_t x, FILE* file);


char* deserialize_string(FILE* file);
void serialize_string(char* str, FILE* file);

// --- magic number related methods

void magic_num_serialize(uint32_t num, FILE* file);


void magic_num_require(uint32_t expected, FILE* file);

#endif
