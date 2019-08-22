package org.vanautrui.languages.symboltablegenerator;

import org.vanautrui.languages.symboltables.tables.*;
import org.vanautrui.languages.symboltables.rows.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.*;
import org.vanautrui.languages.typeresolution.TypeResolver;

public class SymbolTableGenerator {
	public static SubroutineSymbolTable createSubroutineSymbolTable(ClassNode classNode){
		SubroutineSymbolTable subroutineSymbolTable = new SubroutineSymbolTable();

		for(MethodNode methodNode : classNode.methodNodeList){
		    SubroutineSymbolTableRow subrRow = new SubroutineSymbolTableRow(methodNode.methodName.methodName.name,methodNode.type.typeName);
		    subroutineSymbolTable.add(subrRow);
		}
		return subroutineSymbolTable;
	}

	public static LocalVarSymbolTable createMethodScopeSymbolTable(MethodNode methodNode, SubroutineSymbolTable subroutineSymbolTable)throws Exception{
		LocalVarSymbolTable methodScopeSymbolTable=new LocalVarSymbolTable();

		//first, make the local variables for the arguments
		//TODO: figure out if this is correct
		for(DeclaredArgumentNode arg: methodNode.arguments){
		    methodScopeSymbolTable.add(new LocalVarSymbolTableRow(arg.name.name,arg.type.typeName));
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
		                        expressionType
		                )
		        );
		    }
		}
		return methodScopeSymbolTable;
	    }

}
