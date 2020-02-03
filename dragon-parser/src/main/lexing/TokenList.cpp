//standard headers
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

//project headers
#include "TokenList.hpp"

//this is to facilitate special features
//which would be convenient in a token list for our compiler

using namespace std;

TokenList::TokenList(vector<BaseToken> result, string sourceFile) {
	this->tokens = result;
	this->relPath = sourceFile;
}

TokenList::TokenList(TokenList& other) {
	this->relPath = other.relPath;
}

TokenList::TokenList(string path) {
	this->relPath = path;
}

vector<BaseToken> TokenList::getTokens() {
	return this->tokens;
}

void TokenList::add(BaseToken token) {
	this->tokens.push_back(token);
}

void TokenList::addAll(vector<BaseToken> arr) {
	for (BaseToken tk : arr) {
		this->add(tk);
	}
}

void TokenList::consume(int amount) {
	vector<BaseToken> res(this->tokens.begin()+amount,this->tokens.end());
	this->tokens = res;
}

int TokenList::size() {
	return this->tokens.size();
}

bool TokenList::startsWith(BaseToken token) {
	//we cannot rely on .equals()
	//we should use interfaces we can rely on classes to implement
	//the class and the content of the token should be the same for them to be the same

	if (this->tokens.size() > 0) {
		return this->tokens.at(0).tokenEquals(token);
	}
	return false;
}

bool TokenList::endsWith(BaseToken token) {
	if (this->tokens.size() > 0) {
		return this->tokens.at(this->tokens.size() - 1).tokenEquals(token);
	}
	return false;
}

string TokenList::wrap(string s, string wrap) {
	return wrap + s + wrap;
}

void TokenList::expectAndConsumeOtherWiseThrowException(BaseToken token) {
	if (this->size() == 0) {
		throw ("no tokens");
	}

	if (this->startsWith(token)) {
		this->consume(1);
	} else {
		stringstream str;

		str << "\t expected:"
		<< wrap(token.getContents(), "'")
		
		<< " (" + token.kind + ")"

		<< "\t actual:"
		<< wrap(this->head().getContents(), "'")
		
		<< " (" + this->head().kind + ")"

		<< (this->toSourceCodeFragment().substr(0, Math.min(this->toSourceCodeFragment().length(), 100)))

		<<  "Parsing Error: \n"
				<< "\t" << expectedTokenMessage << "\n"
				<< "\t" << actualTokenMessage << "\n"
				<< "in '" << sourceCodeFragment << "'\n"
				<< "in " << (relPath << ":" << this->head().getLineNumber());

		throw to_string(str);
	}
}

TokenList TokenList::copy() {
	return TokenList(this);
}

void TokenList::set(TokenList copy) {
	this->tokens.clear();
	for(BaseToken tk : copy->tokens){
		this->add(tk);
	}
}

BaseToken TokenList::get(int i) {
	return this->tokens.at(i);
}

BaseToken TokenList::head() {
	return this->get(0);
}

string TokenList::toSourceCodeFragment() {
	stringstream str;

	for(BaseToken tk : this->tokens){
		str << tk.getContents();
		str << " ";
	}
	return to_string(str);
}
