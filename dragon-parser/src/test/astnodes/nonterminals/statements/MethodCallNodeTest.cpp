package org.vanautrui.languages.test.astnodes.nonterminals.statements;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LParensToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RParensToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;

import java.nio.file.Paths;

public final class MethodCallNodeTest {

	public static int test1() throws Exception {
		final TokenList list = new TokenList(Paths.get("/dev/null"));

		list.add(new IdentifierToken("main"));
		list.add(new LParensToken());
		list.add(new RParensToken());

		final MethodCallNode call = new MethodCallNode(list);

		boolean assert1 = (0 == list.size());
		return (assert1)?1:0;
	}

	public static int test2() throws Exception {
		final TokenList list = new TokenList(Paths.get("/dev/null"));

		list.add(new IdentifierToken("main"));
		list.add(new LParensToken());
		list.add(new IntegerNonNegativeConstantToken(4));
		list.add(new RParensToken());

		final MethodCallNode call = new MethodCallNode(list);
		boolean assert1 = (0 == list.size());

		return (assert1)?1:0;
	}

	public static int test3() throws Exception {
		final TokenList list = new TokenList(Paths.get("/dev/null"));

		list.add(new IdentifierToken("main"));
		list.add(new LParensToken());
		list.add(new IdentifierToken("x"));
		list.add(new RParensToken());

		final MethodCallNode call = new MethodCallNode(list);
		boolean assert1 = (0 == list.size());
		return (assert1)?1:0;
	}

	public static int test_can_parse_subroutine_call() throws Exception {

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));
		tokens.add(new IdentifierToken("println"));
		tokens.add(new LParensToken());
		tokens.add(new IntegerNonNegativeConstantToken(1));
		tokens.add(new RParensToken());

		final MethodCallNode call = new MethodCallNode(tokens);
		boolean assert1 = (0 == tokens.size());

		return (assert1)?1:0;
	}

	public static int test_can_parse_subroutine_call2() throws Exception {
		//println("x<5")

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));
		tokens.add(new IdentifierToken("println"));
		tokens.add(new LParensToken());
		tokens.add(new StringConstantToken("x<5"));
		tokens.add(new RParensToken());

		final MethodCallNode call = new MethodCallNode(tokens);
		boolean assert1 = (0 == tokens.size());

		return (assert1)?1:0;
	}

	public static int test_can_parse_subroutine_call_with_type_param_argument() throws Exception {
		final TokenList tokens = ParserPhases.makeTokenList("newstruct(4)", false);
		//System.out.println(tokens.toString());

		final MethodCallNode call = new MethodCallNode(tokens);
		boolean assert1 = (0 == tokens.size());
		return (assert1)?1:0;
	}


}
