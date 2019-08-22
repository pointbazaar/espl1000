package org.vanautrui.languages.symboltablegenerator;

import org.vanautrui.languages.codegeneration.symboltables.tables.*;
import org.vanautrui.languages.codegeneration.symboltables.rows.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.*;
import org.vanautrui.languages.typeresolution.DragonTypeResolver;

public class DragonSymbolTableGenerator{
	public static DragonSubroutineSymbolTable createSubroutineSymbolTable(ClassNode classNode){
		DragonSubroutineSymbolTable subroutineSymbolTable = new DragonSubroutineSymbolTable();

		for(MethodNode methodNode : classNode.methodNodeList){
		    DragonSubroutineSymbolTableRow subrRow = new DragonSubroutineSymbolTableRow(methodNode.methodName.methodName.name,methodNode.type.typeName);
		    subroutineSymbolTable.add(subrRow);
		}
		return subroutineSymbolTable;
	}

	public static DragonMethodScopeVariableSymbolTable createMethodScopeSymbolTable(MethodNode methodNode, DragonSubroutineSymbolTable subroutineSymbolTable)throws Exception{
		DragonMethodScopeVariableSymbolTable methodScopeSymbolTable=new DragonMethodScopeVariableSymbolTable();

		//first, make the local variables for the arguments
		//TODO: figure out if this is correct
		for(DeclaredArgumentNode arg: methodNode.arguments){
		    methodScopeSymbolTable.add(new DragonMethodScopeVariableSymbolTableRow(arg.name.name,arg.type.typeName));
		}

		for(StatementNode stmt : methodNode.statements) {

		    //TODO: also get the assignment statements recursively
		    //that are inside the control flow statements.
		    //but that is for later

		    if(stmt.statementNode instanceof AssignmentStatementNode) {
		        AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode)stmt.statementNode;

		        String expressionType = DragonTypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subroutineSymbolTable,methodScopeSymbolTable);

		        methodScopeSymbolTable.add(
		                new DragonMethodScopeVariableSymbolTableRow(
		                        assignmentStatementNode.variableNode.name,
		                        expressionType
		                )
		        );
		    }
		}
		return methodScopeSymbolTable;
	    }

}
