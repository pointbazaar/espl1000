#include "TermTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/Term.hpp"

int term_test_simple_term() {
	try {
		TokenList list = TokenList();
		list.add(INTEGER,"4");
		struct Term* expr = makeTerm(&list,false);
		return 1;
	}catch (string e){
		return 0;
	}
}

int term_test_variable_term() {
	try {
		TokenList list = TokenList();
		list.add(ID,"x");
		struct Term* expr = makeTerm(&list,false);
		return 1;
	}catch (string e){
		return 0;
	}
}


