package org.vanautrui.languages.compiler.symboltablegenerator;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public final class SymbolTableGenerator {

	//prevents instance creation
	private SymbolTableGenerator(){}

	//https://www.youtube.com/watch?v=JHVbSYvK_AQ

	public static SubroutineSymbolTable createSubroutineSymbolTable(final AST_Whole_Program ast, final boolean debug)throws Exception{
		if(debug){
			System.out.println("creating a subroutine symbol table in "+SymbolTableGenerator.class.getSimpleName());
		}

		final SubroutineSymbolTable subroutineSymbolTable = new SubroutineSymbolTable();

		for(final NamespaceNode namespaceNode : ast.namespaceNodeList) {
			if(debug){
				System.out.println(namespaceNode.methods);
			}
			for (final MethodNode methodNode : namespaceNode.methods) {
				if(debug){
					System.out.println("creating subroutine symbol table row for subroutine: "+methodNode.methodName);
				}

				final SubroutineSymbolTableRow subrRow =
								new SubroutineSymbolTableRow(
												methodNode.methodName,
												methodNode.returnType,
												namespaceNode.name,
												count_local_vars(methodNode,subroutineSymbolTable),
												methodNode.arguments.stream().map(a->a.type).collect(Collectors.toList())
								);
				subroutineSymbolTable.add(subrRow);
			}
		}

		return subroutineSymbolTable;
	}

	public static LocalVarSymbolTable createMethodScopeSymbolTable(
			final MethodNode methodNode,
			final SubroutineSymbolTable subTable,
			final StructsSymbolTable structsTable
	)throws Exception{
		final LocalVarSymbolTable varTable=new LocalVarSymbolTable();

		//first, make the local variables for the arguments
		for(final DeclaredArgumentNode arg: methodNode.arguments){

			if(!arg.name.isPresent()){
				throw new Exception("not supported yet");
			}

			varTable.add_idempotent(new LocalVarSymbolTableRow(arg.name.get(), arg.type, LocalVarSymbolTableRow.KIND_ARGUMENT));
		}

		final SymbolTableContext ctx = new SymbolTableContext(subTable,varTable,structsTable);

		for(final StatementNode stmt : methodNode.statements) {
			find_local_vars_recursively(stmt.statementNode,methodNode, ctx);
		}

		return varTable;
	}

	private static int count_local_vars(final MethodNode methodNode,final SubroutineSymbolTable subTable)throws Exception{

		//we cannot build a LocalVarSymbolTable here because we are generating the SubroutineSymbolTable,
		//which would be required to generate a LocalVarSymbolTable

		//we need an approach which just counts the local variables

		//LocalVarSymbolTable localVarSymbolTable = new LocalVarSymbolTable();
		//find_local_vars_recursively_list(methodNode.statements.stream().map(stmt->stmt.statementNode).collect(Collectors.toList()), methodNode,localVarSymbolTable,subTable);

		return find_local_vars_recursively_list_name_only(
				methodNode.statements
					.stream()
					.map(stmt->stmt.statementNode)
					.collect(Collectors.toList()),
				methodNode,
				subTable
		).size();
	}

	private static Set<String> find_local_vars_recursively_list_name_only(
			final List<IStatementNode> stmts,
			final MethodNode methodNode,
			final SubroutineSymbolTable subTable
	)throws Exception{
		final Set<String> result = new HashSet<>();

		for(final IStatementNode stmt : stmts) {
			result.addAll(find_local_vars_recursively_name_only(stmt, methodNode, subTable));
		}
		return result;
	}

	private static Set<String> find_local_vars_recursively_name_only(
			final IStatementNode st,
			final MethodNode methodNode,
			final SubroutineSymbolTable subTable
	)throws Exception{
		final Set<String> result = new HashSet<>();
		if(st instanceof AssignmentStatementNode) {
			final AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode)st;
			if(assignmentStatementNode.variableNode.memberAccessList.size()==0) {
				result.add(assignmentStatementNode.variableNode.simpleVariableNode.name);
			}
		}else{
			//also get the assignment statements recursively
			//that are inside the control flow statements.
			if(st instanceof IfStatementNode) {
				result.addAll(find_local_vars_recursively_list_name_only( ((IfStatementNode) st).statements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,subTable));
				result.addAll(find_local_vars_recursively_list_name_only( ((IfStatementNode) st).elseStatements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,subTable));
			}else if(st instanceof LoopStatementNode) {
				result.addAll(find_local_vars_recursively_list_name_only(((LoopStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, subTable));
			}else if(st instanceof WhileStatementNode) {
				result.addAll(find_local_vars_recursively_list_name_only(((WhileStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, subTable));
			}
		}
		return result;
	}

	private static void find_local_vars_recursively_list(
			final List<IStatementNode> stmts,
			final MethodNode methodNode,
			final SymbolTableContext ctx
	)throws Exception{

		for(final IStatementNode stmt : stmts) {
			find_local_vars_recursively(stmt, methodNode,ctx);
		}
	}

	private static void find_local_vars_recursively(
			final IStatementNode st,
			final MethodNode methodNode,
			final SymbolTableContext ctx
		)throws Exception{

		final SubroutineSymbolTable subTable = ctx.subTable;
		final LocalVarSymbolTable varTable = ctx.varTable;
		final StructsSymbolTable structsTable = ctx.structsTable;

		if(st instanceof AssignmentStatementNode) {
			final AssignmentStatementNode assignStmtNode = (AssignmentStatementNode)st;

			final TypeNode type_of_var =
					(assignStmtNode.optTypeNode.isPresent())?
					assignStmtNode.optTypeNode.get()
					:TypeResolver.getTypeExpressionNode(assignStmtNode.expressionNode, ctx);

			if(assignStmtNode.variableNode.memberAccessList.size()==0) {
				varTable.add_idempotent(
						new LocalVarSymbolTableRow(
								assignStmtNode.variableNode.simpleVariableNode.name,
								type_of_var,
								LocalVarSymbolTableRow.KIND_LOCALVAR
						)
				);
			}
		}else{
			//also get the assignment statements recursively
			//that are inside the control flow statements.

			if(st instanceof IfStatementNode) {
				find_local_vars_recursively_list( ((IfStatementNode) st).statements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,ctx);
				find_local_vars_recursively_list( ((IfStatementNode) st).elseStatements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,ctx);
			}else if(st instanceof LoopStatementNode) {
				find_local_vars_recursively_list(((LoopStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, ctx);
			}else if(st instanceof WhileStatementNode) {
				find_local_vars_recursively_list(((WhileStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, ctx);
			}
		}
	}

	public static StructsSymbolTable createStructsSymbolTable(final AST_Whole_Program ast, final boolean debug) {
		if(debug){
			System.out.println("creating a structs symbol table in "+SymbolTableGenerator.class.getSimpleName());
		}

		final StructsSymbolTable sTable = new StructsSymbolTable();

		for(final NamespaceNode namespaceNode : ast.namespaceNodeList) {
			if(debug){
				System.out.println(namespaceNode.structs);
			}

			for (final StructDeclNode sNode : namespaceNode.structs) {
				if(debug){
					System.out.println("creating struct symbol table row for struct: "+sNode.typeNode.getTypeName());
				}

				final List<String> memberNames = sNode.members.stream().map(x -> x.name).collect(Collectors.toList());
				final List<String> memberTypes = sNode.members.stream().map(x -> x.typeNode.getTypeName()).collect(Collectors.toList());

				final StructsSymbolTableRow row =
						new StructsSymbolTableRow(
								sNode.typeNode.getTypeName(),
								memberNames,
								memberTypes
						);

				sTable.addIfNotAlreadyKnown(row);
			}
		}

		return sTable;
	}
}
