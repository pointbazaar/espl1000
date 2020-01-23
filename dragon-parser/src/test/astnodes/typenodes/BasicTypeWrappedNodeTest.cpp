package org.vanautrui.languages.test.astnodes.typenodes;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

import java.nio.file.Paths;

public final class BasicTypeWrappedNodeTest {

	public static int test_type_parsing2() throws Exception {
		final String source = "( ( ((PInt)~>PInt) )->PInt  )";

		final TokenList list = ParserPhases.makeTokenList(source, false);

		final BasicTypeWrappedNode node = new BasicTypeWrappedNode(list);

		boolean assert1 = (node.typeNode instanceof SubroutineTypeNode);
		boolean assert2 = (0 == list.size());

		return (assert1&&assert2)?1:0;
	}

	public static int test_type_parsing_basic_type_wrapped_node() throws Exception {

		final String source = "( (PInt,NInt,MyType)   ~>  ( (PInt)->PInt )   )";

		final TokenList list = ParserPhases.makeTokenList(source, false);

		final BasicTypeWrappedNode node = new BasicTypeWrappedNode(list);

		boolean assert1 = (node.typeNode instanceof SubroutineTypeNode);
		boolean assert2 = (0 == list.size());

		return (assert1&&assert2)?1:0;
	}

	public static int test_type_parsing_complicated() throws Exception {
		//a subroutine which returns a function which returns a function
		final String innerReturnType = "((PInt)->PInt)";
		final String returnType = "( (PInt)->" + innerReturnType + "   )";
		final String source = "( (PInt,NInt,MyType) ~> " + returnType + "   )  ";

		final TokenList list = ParserPhases.makeTokenList(source, false);

		final BasicTypeWrappedNode node = new BasicTypeWrappedNode(list);

		boolean assert1 = (node.typeNode instanceof SubroutineTypeNode);
		final SubroutineTypeNode mytype = (SubroutineTypeNode) node.typeNode;

		boolean assert2 = (mytype.hasSideEffects);
		boolean assert3 = (3 == mytype.argumentTypes.size());

		final BasicTypeWrappedNode myreturnedtype = (BasicTypeWrappedNode) mytype.returnType.typeNode;


		boolean assert4 = (0 == list.size());

		return (assert1&&assert2&&assert3&&assert4)?1:0;
	}

	public static int test_type_parsing_simple_type() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new TypeIdentifierToken("PInt"));

		final BasicTypeWrappedNode node = new BasicTypeWrappedNode(list);

		boolean assert1 = (node.typeNode instanceof SimpleTypeNode);
		boolean assert2 = (0 == list.size());

		return (assert1&&assert2)?1:0;
	}
}
