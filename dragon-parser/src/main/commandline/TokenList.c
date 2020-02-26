#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "TokenList.h"
#include "TokenKeys.h"

//this is to facilitate special features
//which would be convenient in a token list for our compiler

struct TokenList* makeTokenList_3(struct Token** result, int resultc, char* sourceFile) {
	this->tokens = result;
	this->tokensc = 0;
	this->relPath = sourceFile;
}

struct TokenList* makeTokenList() {
	this->relPath = "/dev/null";
	this->tokensc = 0;
	this->tokens = smalloc(sizeof(struct Token**)*1);
}

/*
void list_add(struct TokenList* list, int token_kind, char* token_value){
	this->tokens.push_back(makeToken(token_kind,token_value));
}

void list_add(struct TokenList* list, int token_kind){
	this->tokens.push_back(makeToken(token_kind));
}
*/
void list_add(struct TokenList* list, struct Token* token) {
	list->tokens.push_back(token);
}

void list_addAll(struct TokenList* list, struct Token** arr, int arrc) {
	for (int i = 0;i < arrc;i++) {
		struct Token* tk = arr[i];
		list_add(list,tk);
	}
}

void list_consume(struct TokenList* list, int amount) {
	list->tokens.erase(list->tokens.begin(),list->tokens.begin()+amount);
}

int list_size(struct TokenList* list, ) {
	return list->tokensc;
}

bool list_startsWith(struct TokenList* list, struct Token* token) {
	//we cannot rely on .equals()
	//we should use interfaces we can rely on classes to implement
	//the class and the content of the token should be the same for them to be the same

	if (list->tokensc > 0) {
		return tokenEquals(list->tokens[0],token);
	}
	return false;
}

bool list_endsWith(struct TokenList* list, struct Token* token) {
	if (list->tokensc > 0) {
		return tokenEquals(list->tokens.at(list->tokensc - 1),token);
	}
	return false;
}

char* wrap(char* s, char* wrap) {
	char res[strlen(s)+2*strlen(wrap)];
	sprintf(res,"%s%s%s",wrap,s,wrap);
	return res;
}

bool list_expect_internal(struct TokenList* list, struct Token* token) {
	//it expect a token or a kind of token, 
	//otherwise throws an exception
	
	if (list_size(list) == 0) {
		return false;
	}

	if (list_startsWith(list, token)) {
		list_consume(list, 1);
		return true;

	} else {
		char str[100];
		str[0]='\0';

		char* reset = "\u001b[0m";

		bool tty = isatty(STDOUT_FILENO);
		//\u001b[36m
		if(tty){
			strcat(str,"\u001b[31m");	//RED
		}

		strcat(str,"Syntax Error ");

		if(tty){
			strcat(str,reset);	
		}

		strcat(str,"in ");

		if(tty){
			strcat(str,"\u001b[36m");	//CYAN
		}

		strcat(str,this->relPath);

		if(tty){
			strcat(str,reset);
		}

		strcat(str,":");
		strcat(str,list_head(list)->lineNumber);
		strcat(str,": expected: ");
		
		strcat(str,token->value);
		
		strcat(str," (");
		strcat(str,token->kind);
		strcat(str,")");

		strcat(str,"\t actual: ");
		strcat(str,list_head(list)->value);
		
		strcat(str, " (");
		strcat(str,list_head(list)->kind);
		strcat(str,")");
		strcat(str,"     ");
		strcat(str,list_code(list));
		
		
		strcat(str,"\n");
		
		return false;
	}

	return true;
}

void list_expect(struct TokenList* list, int token_kind){

	return list_expect_internal(list,makeToken(token_kind));
}

void list_expect(struct TokenList* list, int token_kind, char* token_value){
	return list_expect(list,makeToken_2(token_kind,token_value));
}

struct TokenList* list_copy(struct TokenList* list) {
	struct TokenList* res = smalloc(sizeof(TokenList));

	res->relPath = other.relPath;
	for(int i=0;i<other.tokensc;i++){
		struct Token* tk = other.tokens[i];
		res->tokens.push_back(tk);
	}
	res->tokensc = other ->tokensc;
	return res;
}

void list_set(struct TokenList* list, struct TokenList* copy) {
	list->tokens.clear();
	for(int i=0;i<copy.tokensc;i++){
		struct Token* tk = copy.tokens[i];
		list_add(list, tk);
	}
}

struct Token* list_get(struct TokenList* list, int i) {
	if(list->tokensc <= i){
		//"not enough tokens.";
		return NULL;
	}
	return list->tokens[i];
}

struct Token* list_head(struct TokenList* list) {
	return list_get(list, 0);
}

char* list_code(struct TokenList* list) {
	//it should be a limited fragment 

	char str[100];
	str[0]='\0';

	int i=0;
	while(i < list->tokensc){
		struct Token* tk = list->tokens[i];
		if(i++ < 10){
			strcat(str, tk->value);
		}
	}
	strcat(str,"    ");
	strcat(str,"[");

	i=0;
	while(i < list->tokensc){
		struct Token* tk = list->tokens[i];
		if(i++ < 10){
			strcat(str,tk->kind);
			strcat(str,",");
		}
	}
	strcat(str,"]");

	return str;
}
