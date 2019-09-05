package org.vanautrui.languages.compiler.symboltablegenerator;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public class SymbolTableGenerator {
	public static SubroutineSymbolTable createSubroutineSymbolTable(Set<AST> asts){
		SubroutineSymbolTable subroutineSymbolTable = new SubroutineSymbolTable();
		for(AST ast : asts) {
			for(ClassNode classNode : ast.classNodeList) {
				for (MethodNode methodNode : classNode.methodNodeList) {
					SubroutineSymbolTableRow subrRow = new SubroutineSymbolTableRow(methodNode.methodName.methodName.name, methodNode.type.typeName,classNode.name.typeName);
					subroutineSymbolTable.add(subrRow);
				}
			}
		}
		return subroutineSymbolTable;
	}

	public static LocalVarSymbolTable createMethodScopeSymbolTable(MethodNode methodNode, SubroutineSymbolTable subTable)throws Exception{
		LocalVarSymbolTable methodScopeSymbolTable=new LocalVarSymbolTable();

		//first, make the local variables for the arguments
		for(DeclaredArgumentNode arg: methodNode.arguments){
		    methodScopeSymbolTable.add(new LocalVarSymbolTableRow(arg.name.name,arg.type.typeName,LocalVarSymbolTableRow.KIND_ARGUMENT));
		}
		for(StatementNode stmt : methodNode.statements) {
			find_local_vars_recursively(stmt.statementNode,methodNode, methodScopeSymbolTable, subTable);
		}

		return methodScopeSymbolTable;
	}

	private static void find_local_vars_recursively_list(List<IStatementNode> stmts,MethodNode methodNode,LocalVarSymbolTable methodScopeSymbolTable,SubroutineSymbolTable subTable)throws Exception{
		for(IStatementNode stmt : stmts) {
			find_local_vars_recursively(stmt, methodNode,methodScopeSymbolTable, subTable);
		}
	}

	private static void find_local_vars_recursively(IStatementNode st,MethodNode methodNode,LocalVarSymbolTable methodScopeSymbolTable,SubroutineSymbolTable subTable)throws Exception{

		if(st instanceof AssignmentStatementNode) {
			AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode)st;

			String expressionType = TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,methodScopeSymbolTable);

			methodScopeSymbolTable.add(
					new LocalVarSymbolTableRow(
							assignmentStatementNode.variableNode.name,
							expressionType,
							LocalVarSymbolTableRow.KIND_LOCALVAR
					)
			);
		}else{
			//also get the assignment statements recursively
			//that are inside the control flow statements.

			if(st instanceof IfStatementNode) {
				find_local_vars_recursively_list( ((IfStatementNode) st).statements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,methodScopeSymbolTable,subTable);
				find_local_vars_recursively_list( ((IfStatementNode) st).elseStatements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,methodScopeSymbolTable,subTable);
			}else if(st instanceof LoopStatementNode) {
				find_local_vars_recursively_list(((LoopStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, methodScopeSymbolTable, subTable);
			}else if(st instanceof WhileStatementNode) {
				find_local_vars_recursively_list(((WhileStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, methodScopeSymbolTable, subTable);
			}

		}

	}
}
