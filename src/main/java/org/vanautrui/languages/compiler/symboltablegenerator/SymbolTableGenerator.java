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
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTableRow;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.math.BigInteger;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public class SymbolTableGenerator {
	public static SubroutineSymbolTable createSubroutineSymbolTable(List<AST> asts,boolean debug)throws Exception{
		if(debug){
			System.out.println("creating a subroutine symbol table in "+SymbolTableGenerator.class.getSimpleName());
		}

		SubroutineSymbolTable subroutineSymbolTable = new SubroutineSymbolTable();

		for(AST ast : asts) {
			for(ClassNode classNode : ast.classNodeList) {
				if(debug){
					System.out.println(classNode.methodNodeList);
				}
				for (MethodNode methodNode : classNode.methodNodeList) {
					if(debug){
						System.out.println("creating subroutine symbol table row for subroutine: "+methodNode.methodName);
					}

					SubroutineSymbolTableRow subrRow =
									new SubroutineSymbolTableRow(
													methodNode.methodName,
													methodNode.getType(),classNode.name.getTypeName(),
													count_local_vars(methodNode,subroutineSymbolTable),
													methodNode.arguments.size()
									);
					subroutineSymbolTable.add(subrRow);
				}
			}
		}
		return subroutineSymbolTable;
	}

	public static LocalVarSymbolTable createMethodScopeSymbolTable(MethodNode methodNode, SubroutineSymbolTable subTable)throws Exception{
		LocalVarSymbolTable methodScopeSymbolTable=new LocalVarSymbolTable();

		//first, make the local variables for the arguments
		int arg_index_counter=0;
		for(DeclaredArgumentNode arg: methodNode.arguments){

				methodScopeSymbolTable.add(new LocalVarSymbolTableRow(arg.name, arg.type.typenode, LocalVarSymbolTableRow.KIND_ARGUMENT, arg_index_counter));
		    arg_index_counter++;
		}

		BigInteger local_var_next_index_counter=new BigInteger("0");
		for(StatementNode stmt : methodNode.statements) {
			find_local_vars_recursively(stmt.statementNode,methodNode, methodScopeSymbolTable, subTable,local_var_next_index_counter);
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
			result.add(assignmentStatementNode.variableNode.name);
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



	private static void find_local_vars_recursively_list(List<IStatementNode> stmts,MethodNode methodNode,LocalVarSymbolTable methodScopeSymbolTable,SubroutineSymbolTable subTable,BigInteger local_var_next_index_counter)throws Exception{
		for(IStatementNode stmt : stmts) {
			find_local_vars_recursively(stmt, methodNode,methodScopeSymbolTable, subTable, local_var_next_index_counter);
		}
	}

	private static void find_local_vars_recursively(IStatementNode st, MethodNode methodNode, LocalVarSymbolTable methodScopeSymbolTable, SubroutineSymbolTable subTable, BigInteger local_var_next_index_counter)throws Exception{

		if(st instanceof AssignmentStatementNode) {
			AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode)st;

			ITypeNode expressionType = TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,methodScopeSymbolTable);

			methodScopeSymbolTable.add(
					new LocalVarSymbolTableRow(
							assignmentStatementNode.variableNode.name,
							expressionType,
							LocalVarSymbolTableRow.KIND_LOCALVAR,
							local_var_next_index_counter.intValue()
					)
			);
			local_var_next_index_counter.add(new BigInteger("1"));
		}else{
			//also get the assignment statements recursively
			//that are inside the control flow statements.

			if(st instanceof IfStatementNode) {
				find_local_vars_recursively_list( ((IfStatementNode) st).statements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,methodScopeSymbolTable,subTable,local_var_next_index_counter);
				find_local_vars_recursively_list( ((IfStatementNode) st).elseStatements.stream().map(s->s.statementNode).collect(Collectors.toList()), methodNode,methodScopeSymbolTable,subTable,local_var_next_index_counter);
			}else if(st instanceof LoopStatementNode) {
				find_local_vars_recursively_list(((LoopStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, methodScopeSymbolTable, subTable,local_var_next_index_counter);
			}else if(st instanceof WhileStatementNode) {
				find_local_vars_recursively_list(((WhileStatementNode) st).statements.stream().map(s -> s.statementNode).collect(Collectors.toList()), methodNode, methodScopeSymbolTable, subTable,local_var_next_index_counter);
			}
		}
	}
}
