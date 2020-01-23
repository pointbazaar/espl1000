
//standard headers
#include <vector>

//project headers
#include "BaseToken.hpp"

class TokenList {
public:

	//relative path of the source file
	string relPath;
	vector<BaseToken> tokens;

	TokenList();
	vector<BaseToken> getTokens();
	void add(BaseToken token);
	void addAll(BaseToken[] tokens);
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
}