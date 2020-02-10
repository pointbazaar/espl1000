//standard headers
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

//project headers
#include "TokenList.hpp"

//this is to facilitate special features
//which would be convenient in a token list for our compiler

using namespace std;

TokenList::TokenList(vector<Token> result, string sourceFile) {
	this->tokens = result;
	this->relPath = sourceFile;
}

TokenList::TokenList(TokenList& other) {
	this->relPath = other.relPath;
	for(Token tk : other.tokens){
		this->tokens.push_back(tk);
	}
}

TokenList::TokenList(string path) {
	this->relPath = path;
}

vector<Token> TokenList::getTokens() {
	return this->tokens;
}

void TokenList::add(Token token) {
	this->tokens.push_back(token);
}

void TokenList::addAll(vector<Token> arr) {
	for (Token tk : arr) {
		this->add(tk);
	}
}

void TokenList::consume(int amount) {
	this->tokens.erase(this->tokens.begin(),this->tokens.begin()+amount);
}

int TokenList::size() {
	return this->tokens.size();
}

bool TokenList::startsWith(Token token) {
	//we cannot rely on .equals()
	//we should use interfaces we can rely on classes to implement
	//the class and the content of the token should be the same for them to be the same

	if (this->tokens.size() > 0) {
		return this->tokens.at(0).tokenEquals(token);
	}
	return false;
}

bool TokenList::endsWith(Token token) {
	if (this->tokens.size() > 0) {
		return this->tokens.at(this->tokens.size() - 1).tokenEquals(token);
	}
	return false;
}

string TokenList::wrap(string s, string wrap) {
	return wrap + s + wrap;
}

void TokenList::expect(Token token) {
	//it expect a token or a kind of token, 
	//otherwise throws an exception
	
	if (this->size() == 0) {
		cout << "Error: no tokens" << endl;
		throw "no tokens";
	}

	if (this->startsWith(token)) {
		this->consume(1);
	} else {
		stringstream str;

		str << "\t expected:"
		<< wrap(token.value, "'")
		
		<< " (" << token.kind << ")"

		<< "\t actual:"
		<< wrap(this->head().value, "'")
		
		<< " (" << this->head().kind << ")"

		<< this->code()
			.substr(
				0, 
				min((int)this->code().size(), 100)
			)

		<<  "Parsing Error: \n"
				//<< "\t" << expectedTokenMessage << "\n"
				//<< "\t" << actualTokenMessage << "\n"
				//<< "in '" << sourceCodeFragment << "'\n"
				<< "in " 
				<< this->relPath 
				<< ":" 
				<< this->head().lineNumber;

		cout << "Error: " << str.str() << endl;
		throw str.str();
	}
}

TokenList TokenList::copy() {
	return TokenList(*this);
}

void TokenList::set(TokenList copy) {
	this->tokens.clear();
	for(Token tk : copy.tokens){
		this->add(tk);
	}
}

Token TokenList::get(int i) {
	return this->tokens.at(i);
}

Token TokenList::head() {
	return this->get(0);
}

string TokenList::code() {
	//it should be a limited fragment 

	stringstream str;
	int i=0;
	for(Token tk : this->tokens){
		if(i++ < 10){
			str << tk.value;
			str << "(" << tk.kind << ")";
			str << " ";
		}
	}
	return str.str();
}
