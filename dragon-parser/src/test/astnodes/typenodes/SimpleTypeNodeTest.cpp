package org.vanautrui.languages.test.astnodes.typenodes;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.AnyTypeToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;

import java.nio.file.Paths;

public final class SimpleTypeNodeTest {

	public static int test_typenode_parsing() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new TypeIdentifierToken("MyType"));

		BasicTypeWrappedNode node = new BasicTypeWrappedNode(list);
		boolean assert1 = (0 == list.size());

		return (assert1)?1:0;
	}


	public static int test_typenode_parsing_fails() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("myIllegalType", false);

		try {
			BasicTypeWrappedNode node = new BasicTypeWrappedNode(list);
			return 0;
		} catch (Exception e) {
			return 1;
		}
	}


	public static int test_typenode_parsing_anytype() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new AnyTypeToken());

		final SimpleTypeNode node = new SimpleTypeNode(list);
		boolean assert1 = (0 == list.size());

		return (assert1)?1:0;
	}
}
