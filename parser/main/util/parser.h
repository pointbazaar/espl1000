#pragma once

struct AST* build_ast(int tokensFd, char* filename_display);
struct Namespace* build_namespace(int tokensFd, char* filename_display);
