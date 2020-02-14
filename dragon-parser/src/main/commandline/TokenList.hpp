#ifndef TOKENLIST
#define TOKENLIST

#include <vector>
#include <string>
#include <optional>
#include "Token.hpp"

using namespace std;

class TokenList {
public:

	//relative path of the source file
	string relPath;
	vector<Token> tokens;

	TokenList(TokenList& other);
	TokenList(vector<Token> result, string sourceFile);
	TokenList(vector<Token> tks, int x);
	TokenList(string path);
	TokenList();

	vector<Token> getTokens();
	
	void add(Token token);
	void add(int token_kind);
	void add(int token_kind, string token_value);
	
	void addAll(vector<Token> tokens);

	void consume(int n);

	bool startsWith(Token itk);
	bool endsWith(Token itk);

	string wrap(string s, string wrap);

	void expect(Token itk);
	void expect(int token_kind);
	void expect(int token_kind, string token_value);

	TokenList copy();
	void set(TokenList other);

	Token get(int i);
	Token head();

	int size();


	string code();
};

#endif