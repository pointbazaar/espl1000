package org.vanautrui.languages.symboltablegenerator;

import org.vanautrui.languages.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.symboltables.rows.LocalVarSymbolTableRow;
import org.vanautrui.languages.symboltables.rows.SubroutineSymbolTableRow;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.typeresolution.TypeResolver;

import java.util.Set;

public class SymbolTableGenerator {
	public static SubroutineSymbolTable createSubroutineSymbolTable(Set<AST> asts){
		SubroutineSymbolTable subroutineSymbolTable = new SubroutineSymbolTable();
		for(AST ast : asts) {
			for(ClassNode classNode : ast.classNodeList) {
				for (MethodNode methodNode : classNode.methodNodeList) {
					SubroutineSymbolTableRow subrRow = new SubroutineSymbolTableRow(methodNode.methodName.methodName.name, methodNode.type.typeName);
					subroutineSymbolTable.add(subrRow);
				}
			}
		}
		return subroutineSymbolTable;
	}

	public static LocalVarSymbolTable createMethodScopeSymbolTable(MethodNode methodNode, SubroutineSymbolTable subroutineSymbolTable)throws Exception{
		LocalVarSymbolTable methodScopeSymbolTable=new LocalVarSymbolTable();

		//first, make the local variables for the arguments
		//TODO: figure out if this is correct
		for(DeclaredArgumentNode arg: methodNode.arguments){
		    methodScopeSymbolTable.add(new LocalVarSymbolTableRow(arg.name.name,arg.type.typeName,LocalVarSymbolTableRow.KIND_ARGUMENT));
		}

		for(StatementNode stmt : methodNode.statements) {

		    //TODO: also get the assignment statements recursively
		    //that are inside the control flow statements.
		    //but that is for later

		    if(stmt.statementNode instanceof AssignmentStatementNode) {
		        AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode)stmt.statementNode;

		        String expressionType = TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subroutineSymbolTable,methodScopeSymbolTable);

		        methodScopeSymbolTable.add(
		                new LocalVarSymbolTableRow(
		                        assignmentStatementNode.variableNode.name,
		                        expressionType,
								LocalVarSymbolTableRow.KIND_LOCALVAR
		                )
		        );
		    }
		}
		return methodScopeSymbolTable;
	    }

}
