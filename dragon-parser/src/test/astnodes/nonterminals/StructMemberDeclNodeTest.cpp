package org.vanautrui.languages.test.astnodes.nonterminals;

import org.vanautrui.languages.compiler.lexing.tokens.ArrowToken;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LParensToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RParensToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;

import java.nio.file.Paths;

public final class StructMemberDeclNodeTest {

	public static int test_can_parse_struct_member() throws Exception {
		final String type = "((PInt)~>PInt)";
		final String source = type + " subr";

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

		final StructMemberDeclNode node = new StructMemberDeclNode(list, false);

		final boolean assert1 = "subr".equals( node.name);
		final boolean assert2 = (0 == list.size());

		return (assert1&&assert2)?1:0;
	}
}
