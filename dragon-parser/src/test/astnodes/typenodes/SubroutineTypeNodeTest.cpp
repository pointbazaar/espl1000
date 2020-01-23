package org.vanautrui.languages.test.astnodes.typenodes;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

public final class SubroutineTypeNodeTest {

	public static int test_typename() throws Exception {
		final String source = "(PInt,PInt)->PInt ";
		final TokenList list = ParserPhases.makeTokenList(source, false);

		final SubroutineTypeNode sub = new SubroutineTypeNode(list);

		boolean assert1 = (0 == list.size());

		return (assert1)?1:0;
	}

	public static int test_typename_subroutine_return_type() throws Exception {
		final String source = "(PInt,PInt)->((PInt)->PInt) ";
		final TokenList list = ParserPhases.makeTokenList(source, false);

		final SubroutineTypeNode sub = new SubroutineTypeNode(list);

		boolean assert1 = (0 == list.size());
		return (assert1)?1:0;
	}

	public static int test_subroutine_type_parsing_subroutine_with_side_effects() throws Exception {
		final String source = "(PInt,MyType)~>PInt ";

		final TokenList list = ParserPhases.makeTokenList(source, false);

		final SubroutineTypeNode node = new SubroutineTypeNode(list);
		boolean assert1 = (0 == list.size());
		return (assert1)?1:0;
	}

	public static int test_subroutine_type_parsing_subroutine_without_side_effects() throws Exception {

		final String source = "(PInt)->PInt ";
		final TokenList list = ParserPhases.makeTokenList(source, false);

		SubroutineTypeNode node = new SubroutineTypeNode(list);
		boolean assert1 = (0 == list.size());
		return (assert1)?1:0;
	}

	public static int test_subroutine_type_parsing_subroutine_with_subroutine_argument() throws Exception {
		final String source = "(PInt,((PInt)->PInt))->PInt ";

		final TokenList list = ParserPhases.makeTokenList(source, false);

		SubroutineTypeNode node = new SubroutineTypeNode(list);
		boolean assert1 = (0 == list.size());
		return (assert1)?1:0;
	}
}
