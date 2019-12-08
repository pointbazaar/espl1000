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
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTableRow;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public final class SymbolTableGenerator {
	public static SubroutineSymbolTable createSubroutineSymbolTable(AST_Whole_Program ast, boolean debug)throws Exception{
		if(debug){
			System.out.println("creating a subroutine symbol table in "+SymbolTableGenerator.class.getSimpleName());
		}

		SubroutineSymbolTable subroutineSymbolTable = new SubroutineSymbolTable();

		for(NamespaceNode namespaceNode : ast.namespaceNodeList) {
			if(debug){
				System.out.println(namespaceNode.methodNodeList);
			}
			for (MethodNode methodNode : namespaceNode.methodNodeList) {
				if(debug){
					System.out.println("creating subroutine symbol table row for subroutine: "+methodNode.methodName);
				}

				if(!(methodNode.returnType.type instanceof BasicTypeWrappedNode)){
					throw new Exception("not supported yet");
				}

				SubroutineSymbolTableRow subrRow =
								new SubroutineSymbolTableRow(
												methodNode.methodName,
												((BasicTypeWrappedNode)methodNode.returnType.type).typenode, namespaceNode.name,
												count_local_vars(methodNode,subroutineSymbolTable),
												methodNode.arguments.stream().map(a->a.type).collect(Collectors.toList())
								);
				subroutineSymbolTable.add(subrRow);
			}
		}

		return subroutineSymbolTable;
	}

	public static LocalVarSymbolTable createMethodScopeSymbolTable(
			MethodNode methodNode, SubroutineSymbolTable subTable,StructsSymbolTable structsTable
	)throws Exception{
		final LocalVarSymbolTable methodScopeSymbolTable=new LocalVarSymbolTable();

		//first, make the local variables for the arguments
		for(DeclaredArgumentNode arg: methodNode.arguments){

				if(!(arg.type.type instanceof BasicTypeWrappedNode) || !arg.name.isPresent()){
					throw new Exception("not supported yet");
				}

				methodScopeSymbolTable.add_idempotent(new LocalVarSymbolTableRow(arg.name.get(), (BasicTypeWrappedNode) arg.type.type, LocalVarSymbolTableRow.KIND_ARGUMENT));
		}

		for(StatementNode stmt : methodNode.statements) {
			find_local_vars_recursively(stmt.statementNode,methodNode, methodScopeSymbolTable, subTable, structsTable);
		}

		return methodScopeSymbolTable;
	}

	private static int count_local_vars(MethodNode methodNode,SubroutineSymbolTable subTable)throws Exception{

		//we cannot build a LocalVarSymbolTable here because we are generating the SubroutineSymbolTable,
		//which would be required to generate a LocalVarSymbolTable

		//we need an approach which just counts the local variables

		//LocalVarSymbolTable localVarSymbolTable = new LocalVarSymbolTable();
		//find_local_vars_recursively_list(methodNode.statements.stream().map(stmt->stmt.statementNode).collect(Collectors.toList()), methodNode,localVarSymbolTable,subTable);

		return find_local_vars_recursively_list_name_only(methodNode.statements.stream().map(stmt->stmt.statementNode).collect(Collectors.toList()), methodNode,subTable).size();
	}

	private static Set<String> find_local_vars_recursively_list_name_only(List<IStatementNode> stmts, MethodNode methodNode, SubroutineSymbolTable subTable)throws Exception{
		Set<String> result = new HashSet<>();
		for(IStatementNode stmt : stmts) {
			result.addAll(find_local_vars_recursively_name_only(stmt, methodNode, subTable));
		}
		return result;
	}

	private static Set<String> find_local_vars_recursively_name_only(IStatementNode st,MethodNode methodNode,SubroutineSymbolTable subTable)throws Exception{
		Set<String> result = new HashSet<>();
		if(st instanceof AssignmentStatementNode) {
			AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode)st;
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
			List<IStatementNode> stmts, MethodNode methodNode, LocalVarSymbolTable methodScopeSymbolTable, SubroutineSymbolTable subTable, StructsSymbolTable structsTable
	)throws Exception{
		for(IStatementNode stmt : stmts) {
			find_local_vars_recursively(stmt, methodNode,methodScopeSymbolTable, subTable,structsTable);
		}
	}

	private static void find_local_vars_recursively(
			IStatementNode st,
			MethodNode methodNode,
			LocalVarSymbolTable methodScopeSymbolTable,
			SubroutineSymbolTable subTable,
			StructsSymbolTable structsTable
	)throws Exception{

		if(st instanceof AssignmentStatementNode) {
			final AssignmentStatementNode assignStmtNode = (AssignmentStatementNode)st;

			TypeNode type_of_var;

			if(assignStmtNode.optTypeNode.isPresent()){
				type_of_var=assignStmtNode.optTypeNode.get();
			}else {
				type_of_var = TypeResolver.getTypeExpressionNode(assignStmtNode.expressionNode, subTable, methodScopeSymbolTable, structsTable);
			}

			if(assignStmtNode.variableNode.memberAccessList.size()==0) {
				methodScopeSymbolTable.add_idempotent(
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
				find_local_vars_recursively_list( ((IfStatementNode) st).statements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,methodScopeSymbolTable,subTable,structsTable);
				find_local_vars_recursively_list( ((IfStatementNode) st).elseStatements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,methodScopeSymbolTable,subTable,structsTable);
			}else if(st instanceof LoopStatementNode) {
				find_local_vars_recursively_list(((LoopStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, methodScopeSymbolTable, subTable,structsTable);
			}else if(st instanceof WhileStatementNode) {
				find_local_vars_recursively_list(((WhileStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, methodScopeSymbolTable, subTable,structsTable);
			}
		}
	}

	public static StructsSymbolTable createStructsSymbolTable(AST_Whole_Program ast, boolean debug) throws Exception{
		if(debug){
			System.out.println("creating a structs symbol table in "+SymbolTableGenerator.class.getSimpleName());
		}

		StructsSymbolTable sTable = new StructsSymbolTable();

		for(NamespaceNode namespaceNode : ast.namespaceNodeList) {
			if(debug){
				System.out.println(namespaceNode.structDeclNodeList);
			}

			for (StructDeclNode sNode : namespaceNode.structDeclNodeList) {
				if(debug){
					System.out.println("creating struct symbol table row for struct: "+sNode.getTypeName());
				}

				final List<String> memberNames = sNode.structMembersList.stream().map(x -> x.name).collect(Collectors.toList());
				final List<String> memberTypes = sNode.structMembersList.stream().map(x -> x.type.getTypeName()).collect(Collectors.toList());

				final StructsSymbolTableRow row =
						new StructsSymbolTableRow(
								sNode.getTypeName(),
								memberNames,
								memberTypes
						);

				sTable.addIfNotAlreadyKnown(row);
			}
		}

		return sTable;
	}
}
