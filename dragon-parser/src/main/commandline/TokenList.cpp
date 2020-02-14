//standard headers
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <unistd.h>

//project headers
#include "TokenList.hpp"
#include "TokenKeys.hpp"

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

TokenList::TokenList() {
	this->relPath = "/dev/null";
}

vector<Token> TokenList::getTokens() {
	return this->tokens;
}

void TokenList::add(int token_kind, string token_value){
	this->tokens.push_back(Token(token_kind,token_value));
}

void TokenList::add(int token_kind){
	this->tokens.push_back(Token(token_kind));
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
		throw string("Error: no tokens");
	}

	if (this->startsWith(token)) {
		this->consume(1);
	} else {
		stringstream str;

		string reset = "\u001b[0m";

		bool tty = isatty(STDOUT_FILENO);
		//\u001b[36m
		if(tty){
			str << "\u001b[31m";	//RED
		}

		str << "Syntax Error ";

		if(tty){
			str << reset;	
		}

		str << "in ";

		if(tty){
			str << "\u001b[36m";	//CYAN
		}

		str << this->relPath ;

		if(tty){
			str << reset;
		}

		str << ":" << this->head().lineNumber  <<": expected: "
		
		<< token.value
		
		<< " (" << token.kind << ")"

		<< "\t actual: "
		<< this->head().value
		
		<< " (" << this->head().kind << ")"
		<< "     "
		<< this->code()
		
		
		<< endl;
		
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
	if(this->tokens.size() <= i){
		throw string("not enough tokens.");
	}
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
		}
	}
	str << "    ";
	str << "[";
	i=0;
	for(Token tk : this->tokens){
		if(i++ < 10){
			str  << tk.kind;
			str << ",";
		}
	}
	str << "]";
	return str.str();
}
