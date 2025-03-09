#pragma once

// This analyzer module visits the AST
// to look for StringConst instances
// and adds them to DataTable

struct ST;
struct AST;

void analyze_data(struct ST* st, struct AST* ast);
