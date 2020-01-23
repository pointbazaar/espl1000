package org.vanautrui.languages.test.astnodes.nonterminals.statements;

import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.compiler.lexing.tokens.StructMemberAccessToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LParensToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RParensToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.SemicolonToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;

import java.nio.file.Paths;

public final class StatementNodeTest {

	public static int test_assignment_statement_with_struct_access() throws Exception {

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));
		tokens.add(new IdentifierToken("x"));
		tokens.add(new StructMemberAccessToken());
		tokens.add(new IdentifierToken("a"));
		tokens.add(new OperatorToken("="));
		tokens.add(new IntegerNonNegativeConstantToken(3));
		tokens.add(new SemicolonToken());

		final StatementNode node = new StatementNode(tokens);

		boolean assert1 = (0 == tokens.size()); //all tokens should have been consumed
		return (assert1)?1:0;
	}

	public static int test_assignment_statement_with_method_call() throws Exception {

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));

		tokens.add(new IdentifierToken("flip"));
		tokens.add(new LParensToken());
		tokens.add(new IdentifierToken("x"));
		tokens.add(new RParensToken());
		tokens.add(new SemicolonToken());

		final StatementNode node = new StatementNode(tokens);

		boolean assert1 = (0 == tokens.size()); //all tokens should have been consumed

		return (assert1)?1:0;
	}
}
