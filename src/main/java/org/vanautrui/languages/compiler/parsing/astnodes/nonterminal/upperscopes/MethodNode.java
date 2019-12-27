package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class MethodNode implements IASTNode {

	public final boolean isPublic = true;

	public final boolean has_side_effects;

	public final TypeNode returnType;

	public final String methodName;

	public final List<DeclaredArgumentNode> arguments = new ArrayList<>();

	public final List<StatementNode> statements = new ArrayList<>();

	public MethodNode(final TypeNode returnType, final String methodName) {
		this.returnType = returnType;
		this.methodName = methodName;
		this.has_side_effects = false;
	}

	public MethodNode(final TypeNode returnType, final String methodName, final List<StatementNode> statements) {
		this.returnType = returnType;
		this.methodName = methodName;
		this.has_side_effects = false;
		this.statements.addAll(statements);
	}

	@Override
	public String toSourceCode() {

		return
				"fn "
						+ this.methodName
						+ " ("
						+ this.arguments.stream()
						.map(argument -> argument.toSourceCode())
						.collect(Collectors.joining(","))
						+ ") -> "
						+ this.returnType.toSourceCode()
						+ " {"
						+ "\n"
						+ this.statements.stream()
						.map(statement -> "\t" + statement.toSourceCode())
						.collect(Collectors.joining("\n"))
						+ "\n"
						+ "}"
				;
	}
}
