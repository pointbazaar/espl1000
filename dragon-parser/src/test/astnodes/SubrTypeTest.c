#include "SubrTypeTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/typenodes/SubrType.h"

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

	struct SubrType* sub = makeSubrType(&l,false);

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

	struct SubrType* sub = makeSubrType(&l,false);

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

	struct SubrType* node = makeSubrType(&l,false);

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

	struct SubrType* node = makeSubrType(&l,false);

	bool assert1 = (0 == l.size());
	return (assert1)?1:0;
}

