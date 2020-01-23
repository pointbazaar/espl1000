package org.vanautrui.languages.test.astnodes.nonterminals.statements.upperscopes;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

public final class MethodNodeTest {

	public static int test_can_parse_method_with_arguments() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("fn main (String hello)~>PInt { main(); }", false);

		MethodNode methodNode = new MethodNode(list, false);

		return  methodNode.arguments.size();
	}

	public static int test_can_parse_method_with_subroutine_argument() throws Exception {
		final TokenList list = ParserPhases.makeTokenList("fn main ((()~>PInt) subr)~>PInt { return subr(); }", false);

		MethodNode methodNode = new MethodNode(list, false);

		return  methodNode.arguments.size();
	}

	public static int test_can_parse_subroutine() {

		try {
			final TokenList tokens = ParserPhases.makeTokenList(
					"fn main ()~>PInt {" +
					"println(1);" +
					"return 0;" +
					"}",
					false);

			final MethodNode methodNode = new MethodNode(tokens, false);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_can_parse_method_without_arguments() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("fn main ()~>PInt { main(); } ", false);

		MethodNode mthd = new MethodNode(list, false);
		boolean assert1 = (0 == mthd.arguments.size());

		return (assert1)?1:0;
	}

	public static int test2() throws Exception {
		final String src = "fn main ()~>PInt { x = 3 ; if ( x < 5 ) { println ( x<5 ) ; } return 0 ; } ";
		final TokenList list = ParserPhases.makeTokenList(src, false);

		MethodNode mthd = new MethodNode(list, false);
		boolean assert1 = (0 == mthd.arguments.size());

		return (assert1)?1:0;
	}

	public static int test_parse_methodnode_with_struct_access_statements() throws Exception {
		final TokenList tokens = ParserPhases.makeTokenList("fn main ( ) ~> PInt { x = flip ( y ) ; x . a = 3 ; } ", false);

		final MethodNode node = new MethodNode(tokens, false);

		boolean assert1 = (0 == tokens.size()); //all tokens should have been consumed

		return (assert1)?1:0;
	}

}
