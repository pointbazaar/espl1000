package org.vanautrui.languages.test.astnodes.nonterminals;

import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.StructMemberAccessToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LBracketToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RBracketToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;

import java.nio.file.Paths;

public final class VariableNodeTest {

	public static int test_parse_struct_member_access() throws Exception {

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));
		tokens.add(new IdentifierToken("x"));
		tokens.add(new StructMemberAccessToken());
		tokens.add(new IdentifierToken("a"));

		final VariableNode node = new VariableNode(tokens);

		boolean assert1 = (1 == node.memberAccessList.size());
		boolean assert2 = "x".equals( node.simpleVariableNode.name);
		boolean assert3 = !(node.simpleVariableNode.indexOptional.isPresent());
		boolean assert4 = "a".equals( node.memberAccessList.get(0).simpleVariableNode.name);
		boolean assert5 = !(node.memberAccessList.get(0).simpleVariableNode.indexOptional.isPresent());

		return (assert1&&assert2&&assert3&&assert4&&assert5)?1:0;
	}

	public static int test_parse_index_access() throws Exception {

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));
		tokens.add(new IdentifierToken("x"));
		tokens.add(new LBracketToken());
		tokens.add(new IntegerNonNegativeConstantToken(0));
		tokens.add(new RBracketToken());

		final VariableNode node = new VariableNode(tokens);

		boolean assert1 = (0 == node.memberAccessList.size());
		boolean assert2 = "x".equals(node.simpleVariableNode.name);
		boolean assert3 = (node.simpleVariableNode.indexOptional.isPresent());

		return (assert1&&assert2&&assert3)?1:0;
	}

	public static int test_parse_struct_member_access_and_index_access() throws Exception {

		//final TokenList tokens = ParserPhases.makeTokenList("x.a[0].b");

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));
		tokens.add(new IdentifierToken("x"));
		tokens.add(new StructMemberAccessToken());
		tokens.add(new IdentifierToken("a"));

		tokens.add(new LBracketToken());
		tokens.add(new IntegerNonNegativeConstantToken(0));
		tokens.add(new RBracketToken());

		tokens.add(new StructMemberAccessToken());
		tokens.add(new IdentifierToken("b"));

		final VariableNode node = new VariableNode(tokens);

		boolean assert1 = (1 == node.memberAccessList.size());
		boolean assert2 = "x".equals(node.simpleVariableNode.name);

		boolean assert3 = "a".equals( node.memberAccessList.get(0).simpleVariableNode.name);
		boolean assert4 = (node.memberAccessList.get(0).simpleVariableNode.indexOptional.isPresent());

		return (assert1&&assert2&&assert3&&assert4)?1:0;
	}
}
