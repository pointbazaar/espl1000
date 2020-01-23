package org.vanautrui.languages.test.astnodes.nonterminals;

import org.vanautrui.languages.compiler.lexing.tokens.ArrowToken;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LParensToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RParensToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;

import java.nio.file.Paths;

public final class DeclaredArgumentNodeTest {

	public static int test_parse_declared_argument() throws Exception {
		final String source = "((PInt)~>PInt) subr";

		//final TokenList list = ParserPhases.makeTokenList(source);

		final TokenList list = new TokenList(Paths.get("/dev/null"));

		list.add(new LParensToken());
		list.add(new LParensToken());
		list.add(new TypeIdentifierToken("PInt"));
		list.add(new RParensToken());
		list.add(new ArrowToken(true, false));
		list.add(new TypeIdentifierToken("PInt"));
		list.add(new RParensToken());
		list.add(new IdentifierToken("subr"));

		DeclaredArgumentNode node = new DeclaredArgumentNode(list);

		final boolean assert1 = "subr".equals( node.name.get());
		final boolean assert2 = (0 == list.size());

		return (assert1&&assert2)?1:0;
	}
}
