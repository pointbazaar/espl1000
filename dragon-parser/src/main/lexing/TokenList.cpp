//standard headers
#include <vector>
#include <iostream>
#include <fstream>

//project headers
#include "TokenList.hpp"

//this is to facilitate special features
//which would be convenient in a token list for our compiler

using namespace std;

TokenList::TokenList(vector<BaseToken> result, Path sourceFile) {
	this.tokens = result;
	this.relPath = sourceFile;
}

TokenList::TokenList(TokenList other) {
	this.tokens = new ArrayList<>(other.tokens);
	this.relPath = other.relPath;
}

TokenList::TokenList(Path path) {
	this.tokens = new ArrayList<>();
	this.relPath = path;
}

vector<BaseToken> TokenList::getTokens() {
	return this.tokens;
}

void TokenList::add(BaseToken token) {
	this.tokens.add(token);
}

void TokenList::addAll(vector<BaseToken> arr) {
	for (BaseToken tk : arr) {
		this.add(tk);
	}
}

void TokenList::consume(int amount) {
	this.tokens = this.tokens.subList(amount, this.tokens.size());
}

int TokenList::size() {
	return this.tokens.size();
}

bool TokenList::startsWith(BaseToken token) {
	//we cannot rely on .equals()
	//we should use interfaces we can rely on classes to implement
	//the class and the content of the token should be the same for them to be the same

	if (this.tokens.size() > 0) {
		return this.tokens.get(0).tokenEquals(token);
	}
	return false;
}

bool TokenList::endsWith(BaseToken token) {
	if (this.tokens.size() > 0) {
		return this.tokens.get(this.tokens.size() - 1).tokenEquals(token);
	}
	return false;
}

string TokenList::wrap(string s, string wrap) {
	return wrap + s + wrap;
}

void TokenList::expectAndConsumeOtherWiseThrowException(BaseToken token) {
	if (this.size() == 0) {
		throw ("no tokens");
	}

	if (this.startsWith(token)) {
		this.consume(1);
	} else {
		string expectedStart = String.format("\t%-20s", "expected:");
		string expectedEnd = String.format("%-20s", wrap(token.getContents(), "'"));
		string expectedTokenMessage = expectedStart + expectedEnd + " (" + token.getClass().getSimpleName() + ")";

		string actualStart = String.format("\t%-20s", "actual:");
		string actualEnd = String.format("%-20s", wrap(this.head().getContents(), "'"));
		string actualTokenMessage = actualStart + actualEnd + " (" + this.head().getClass().getSimpleName() + ")";

		string sourceCodeFragment = (this.toSourceCodeFragment().substring(0, Math.min(this.toSourceCodeFragment().length(), 100)));

		string message = "Parsing Error: \n"
				+ "\t" + expectedTokenMessage + "\n"
				+ "\t" + actualTokenMessage + "\n"
				+ "in '" + sourceCodeFragment + "'\n"
				+ "in " + (relPath + ":" + this.head().getLineNumber());

		throw message;
	}
}

TokenList TokenList::copy() {
	return new TokenList(this);
}

void TokenList::set(TokenList copy) {
	this.tokens.clear();
	this.tokens.addAll(copy.tokens);
}

BaseToken TokenList::get(int i) {
	return this.tokens.get(i);
}

BaseToken TokenList::head() {
	return this.get(0);
}

string TokenList::toSourceCodeFragment() {
	return this.tokens
		.stream()
		.map(token -> token.getContents())
		.collect(Collectors.joining(" "));
}
