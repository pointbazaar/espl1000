#include "SubrTypeTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/typenodes/SubrType.hpp"

int subrtype_test_typename() {
	//(PInt,PInt)->PInt
	TokenList l = TokenList();

	l.add(LPARENS);
	l.add(TYPEIDENTIFIER,"PInt");
	l.add(COMMA);
	l.add(TYPEIDENTIFIER,"PInt");
	l.add(RPARENS);
	l.add(ARROW);
	l.add(TYPEIDENTIFIER,"PInt");

	SubrType* sub = new SubrType(l,false);

	bool assert1 = (0 == l.size());

	return (assert1)?1:0;
}

int subrtype_test_typename_subroutine_return_type() {
	//(PInt,PInt)->((PInt)->PInt) 
	TokenList l = TokenList();

	l.add(LPARENS);
		l.add(TYPEIDENTIFIER,"PInt");
		l.add(COMMA);
		l.add(TYPEIDENTIFIER,"PInt");
	l.add(RPARENS);

	l.add(ARROW);

	l.add(LPARENS);
		l.add(LPARENS);
			l.add(TYPEIDENTIFIER,"PInt");
		l.add(RPARENS);

		l.add(ARROW);
		l.add(TYPEIDENTIFIER,"PInt");
	l.add(RPARENS);

	SubrType* sub = new SubrType(l,false);

	bool assert1 = (0 == l.size());
	return (assert1)?1:0;
}

int subrtype_test_subroutine_type_parsing_subroutine_with_side_effects()  {
	//(PInt,MyType)~>PInt
	TokenList l = TokenList();

	l.add(LPARENS);
	l.add(TYPEIDENTIFIER,"PInt");
	l.add(COMMA);
	l.add(TYPEIDENTIFIER,"MyType");
	l.add(RPARENS);
	l.add(ARROW);
	l.add(TYPEIDENTIFIER,"PInt");

	SubrType* node = new SubrType(l,false);
	bool assert1 = (0 == l.size());
	return (assert1)?1:0;
}

int subrtype_test_subroutine_type_parsing_subroutine_without_side_effects() {
	//(PInt)->PInt
	TokenList l = TokenList(); 

	l.add(LPARENS);
	l.add(TYPEIDENTIFIER,"PInt");
	l.add(RPARENS);
	l.add(ARROW);
	l.add(TYPEIDENTIFIER,"PInt");

	SubrType* node = new SubrType(l,false);
	bool assert1 = (0 == l.size());
	return (assert1)?1:0;
}

