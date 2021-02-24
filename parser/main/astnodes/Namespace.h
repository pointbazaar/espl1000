#ifndef NAMESPACE
#define NAMESPACE

struct TokenList;
struct Namespace;

struct Namespace* makeNamespace(struct TokenList* tokens, char* name, bool debug);

void ns_parse_methods(struct Namespace* res, struct TokenList* copy, bool debug);
void ns_parse_structs(struct Namespace* res, struct TokenList* copy, bool debug);

#endif
