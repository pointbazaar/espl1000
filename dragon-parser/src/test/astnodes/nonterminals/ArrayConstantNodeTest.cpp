package org.vanautrui.languages.test.astnodes.nonterminals;

import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.CommaToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LBracketToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RBracketToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;

import java.nio.file.Paths;

public final class ArrayConstantNodeTest {

	public static int test_empty_array() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));

		list.add(new LBracketToken());
		list.add(new RBracketToken());
		final ArrayConstantNode arrayConstantNode = new ArrayConstantNode(list);

		boolean assert1 = (0 == arrayConstantNode.elements.size());

		return (assert1)?1:0;
	}

	public static int test_simple_array() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));

		list.add(new LBracketToken());
		list.add(new IntegerNonNegativeConstantToken(1));
		list.add(new RBracketToken());

		final ArrayConstantNode arrayConstantNode = new ArrayConstantNode(list);

		final boolean assert1 = (1 == arrayConstantNode.elements.size());

		return (assert1)?1:0;
	}

	public static int test_array_multiple_elements() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));

		list.add(new LBracketToken());
		list.add(new IntegerNonNegativeConstantToken(1));
		list.add(new CommaToken());
		list.add(new IntegerNonNegativeConstantToken(2));
		list.add(new RBracketToken());

		final ArrayConstantNode arrayConstantNode = new ArrayConstantNode(list);

		final boolean assert1 = (2 == arrayConstantNode.elements.size());

		return (assert1)?1:0;
	}
}
