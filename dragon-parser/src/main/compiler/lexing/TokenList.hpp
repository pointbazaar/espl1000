#ifndef TOKENLIST
#define TOKENLIST

//standard headers
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <optional>

//project headers
#include "BaseToken.hpp"

using namespace std;

class TokenList {
public:

	//relative path of the source file
	string relPath;
	vector<BaseToken> tokens;

	TokenList();
	vector<BaseToken> getTokens();
	void add(BaseToken token);
	void addAll(vector<BaseToken> tokens);
	void consume(int n);
	bool startsWith(BaseToken itk);
	bool endsWith(BaseToken itk);
	string wrap(string s, string wrap);
	void expectAndConsumeOtherWiseThrowException(BaseToken itk);
	TokenList copy();
	void set(TokenList other);
	BaseToken get(int i);
	BaseToken head();
	string toSourceCodeFragment();
};

#endif