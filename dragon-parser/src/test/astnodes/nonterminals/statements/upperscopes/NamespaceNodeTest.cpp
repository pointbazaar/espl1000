package org.vanautrui.languages.test.astnodes.nonterminals.statements.upperscopes;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;

public final class NamespaceNodeTest {

	public static int test_can_parse_namespace_with_1_empty_struct() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("struct MyStruct{}", false);

		NamespaceNode namespaceNode = new NamespaceNode(list, "Main", false);

		//assertEquals(namespaceNode.name,"Main");

		boolean assert1 = (1 == namespaceNode.structs.size());
		boolean assert2 = (0 == namespaceNode.structs.get(0).members.size());

		return (assert1&&assert2)?1:0;
	}

	public static int test_can_parse_namespace_with_1_empty_method() throws Exception {

		//TODO:
		//these parser tests are problematic, because they don't just test the parser,
		//they also test the lexer.
		//i should write the tests so that the parser alone can accomplic shem

		final TokenList list = ParserPhases.makeTokenList("fn main ()~>PInt {} ", false);

		final NamespaceNode namespaceNode = new NamespaceNode(list, "Main", false);

		boolean assert1 = "Main".equals(namespaceNode.name);
		boolean assert2 = (1 == namespaceNode.methods.size());

		return (assert1&&assert2)?1:0;
	}

	public static int test_can_parse_namespace_with_1_method() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("fn main ()~>PInt {main();} ", false);

		final NamespaceNode namespaceNode = new NamespaceNode(list, "Main", false);

		boolean assert1 = namespaceNode.name.equals( "Main");
		boolean assert2 = (1 == namespaceNode.methods.size());

		final MethodNode methodNode = namespaceNode.methods.get(0);

		boolean assert3 = "main".equals( methodNode.methodName);
		boolean assert4 = (methodNode.hasSideEffects);

		return (assert1&&assert2&&assert3&&assert4)?1:0;
	}

	public static int test_can_call_subroutine_argument() throws Exception {

		try {
			final String method1 = " fn id ( PInt n ) -> PInt {  return n; } ";
			final String method2 = " fn subr ( ( ( PInt ) ~> PInt ) subr2)->PInt {  return subr2(2); } ";
			final String method3 = " fn main ( ) ~> PInt{ putdigit(subr(id)); return 0;} ";

			final MethodNode method1Node = new MethodNode(ParserPhases.makeTokenList(method1, false), false);
			final MethodNode method2Node = new MethodNode(ParserPhases.makeTokenList(method2, false), false);
			final MethodNode method3Node = new MethodNode(ParserPhases.makeTokenList(method3, false), false);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_can_parse_namespace_subroutine_calling_subroutine_argument() throws Exception {

		final TokenList list = ParserPhases.makeTokenList(" fn main  ()~>PInt {return subr(id);} fn id (PInt n)->PInt {return n;} fn subr (((PInt)->PInt) f)~>PInt {return f(3);} ", false);

		NamespaceNode namespaceNode = new NamespaceNode(list, "Main", false);

		boolean assert1 = namespaceNode.name.equals( "Main");
		boolean assert2 = (3 == namespaceNode.methods.size());

		return (assert1&&assert2)?1:0;
	}
}
