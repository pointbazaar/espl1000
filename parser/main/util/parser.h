#ifndef PARSER_H
#define PARSER_H

struct AST* build_ast(char* tokensFile);
struct Namespace* build_namespace(char* tokensFile);
#endif