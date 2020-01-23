package org.vanautrui.languages.test.astnodes.nonterminals.statements;

import org.vanautrui.languages.compiler.lexing.tokens.*;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.*;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;

import java.nio.file.Paths;

public final class  AssignmentStatementNodeTest {

	public static int test1() {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));

			tokens.add(new IdentifierToken("x"));
			tokens.add(new OperatorToken("="));
			tokens.add(new IntegerNonNegativeConstantToken(4));
			tokens.add(new SemicolonToken());

			AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_assign_method_call_result() {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));

			tokens.add(new IdentifierToken("x"));
			tokens.add(new OperatorToken("="));

			//nop refers to the no operation method. it does nothing with its argument
			tokens.add(new IdentifierToken("nop"));

			tokens.add(new LParensToken());
			tokens.add(new IntegerNonNegativeConstantToken(4));
			tokens.add(new RParensToken());

			tokens.add(new SemicolonToken());

			AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_assign_method_call_result_2() {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));

			tokens.add(new IdentifierToken("x"));
			tokens.add(new OperatorToken("="));

			//nop refers to the no operation method. it does nothing with its argument
			tokens.add(new IdentifierToken("nop"));

			tokens.add(new LParensToken());
			tokens.add(new RParensToken());

			tokens.add(new SemicolonToken());

			final AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_assign_variable_with_array_index() {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));

			tokens.add(new IdentifierToken("x"));
			tokens.add(new OperatorToken("="));

			//nop refers to the no operation method. it does nothing with its argument
			tokens.add(new IdentifierToken("arr"));

			tokens.add(new LBracketToken());
			tokens.add(new IntegerNonNegativeConstantToken(4));
			tokens.add(new RBracketToken());

			tokens.add(new SemicolonToken());

			final AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_assign_char() throws Exception {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));

			tokens.add(new IdentifierToken("x"));
			tokens.add(new OperatorToken("="));

			tokens.add(new CharConstantToken('x'));

			tokens.add(new SemicolonToken());

			final AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_can_assign_to_struct_member() throws Exception {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));
			tokens.add(new IdentifierToken("x"));
			tokens.add(new StructMemberAccessToken());
			tokens.add(new IdentifierToken("a"));
			tokens.add(new OperatorToken("="));
			tokens.add(new IntegerNonNegativeConstantToken(3));
			tokens.add(new SemicolonToken());

			final AssignmentStatementNode node = new AssignmentStatementNode(tokens);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_type_declaration_for_variable() throws Exception {

		final TokenList tokens = new TokenList(Paths.get("/dev/null"));
		tokens.add(new TypeIdentifierToken("PInt"));
		tokens.add(new IdentifierToken("x"));
		tokens.add(new OperatorToken("="));
		tokens.add(new IntegerNonNegativeConstantToken(3));
		tokens.add(new SemicolonToken());

		final AssignmentStatementNode node = new AssignmentStatementNode(tokens);
		boolean assert1 = (node.optTypeNode.isPresent());
		boolean assert2 = (0 == node.variableNode.memberAccessList.size());
		boolean assert3 = (0 == tokens.size());

		return (assert1&&assert2&&assert3)?1:0;
	}
}
