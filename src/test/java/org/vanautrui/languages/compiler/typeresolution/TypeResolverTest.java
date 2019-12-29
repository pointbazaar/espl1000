package org.vanautrui.languages.compiler.typeresolution;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

public final class TypeResolverTest {

	//TODO: add a whole lotta more tests.
	//TODO: add a whole lot more difficult tests.

	@Test
	public void test_resolveSimpleExpressionType() throws Exception {

		final TypeNode returnType = new TypeNode(new SimpleTypeNode("PInt"));

		final ExpressionNode expr = new ExpressionNode(
				new TermNode(new IntConstNode(3)),
				new OperatorNode("+"),
				new TermNode(new IntConstNode(3))
		);

		final List<StatementNode> statements = new ArrayList<>();
		statements.add(new StatementNode(
			new AssignmentStatementNode(
						new VariableNode(new SimpleVariableNode("x")),
							expr
				)
		));

		final MethodNode mymethod = new MethodNode(returnType,"main",statements);

		mymethod.arguments = new ArrayList<>();

		final AST_Whole_Program ast = new AST_Whole_Program(new NamespaceNode(mymethod,"Main"));

		final StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(ast,false);
		final SubroutineSymbolTable subTable = SymbolTableGenerator.createSubroutineSymbolTable(ast,false);
		final LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(mymethod,subTable,structsTable);

		final SymbolTableContext ctx = new SymbolTableContext(subTable,varTable,structsTable);


		final TypeNode typeExpressionNode = TypeResolver.getTypeExpressionNode(expr, ctx);
		
		Assert.assertEquals("PInt",typeExpressionNode.getTypeName());
	}
}
