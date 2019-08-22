package org.vanautrui.languages.symboltablegenerator;

import org.objectweb.asm.*;
import org.vanautrui.languages.codegeneration.symboltables.nameconversions.TypeNameToJVMInternalTypeNameConverter;
import org.vanautrui.languages.codegeneration.symboltables.tables.*;
import org.vanautrui.languages.codegeneration.symboltables.rows.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.*;
import org.vanautrui.languages.typeresolution.DragonTypeResolver;
import java.util.stream.Collectors;
import static org.objectweb.asm.Opcodes.*;

public class DragonSymbolTableGenerator{
	public static DragonSubroutineSymbolTable createSubroutineSymbolTable(DragonClassNode classNode){
		DragonSubroutineSymbolTable subroutineSymbolTable = new DragonSubroutineSymbolTable();

		for(DragonMethodNode methodNode : classNode.methodNodeList){
		    DragonSubroutineSymbolTableRow subrRow = new DragonSubroutineSymbolTableRow(methodNode.methodName.methodName.name,methodNode.type.typeName);
		    subroutineSymbolTable.add(subrRow);
		}
		return subroutineSymbolTable;
	}

	public static DragonMethodScopeVariableSymbolTable createMethodScopeSymbolTable(DragonMethodNode methodNode,DragonSubroutineSymbolTable subroutineSymbolTable)throws Exception{
		DragonMethodScopeVariableSymbolTable methodScopeSymbolTable=new DragonMethodScopeVariableSymbolTable();

		//first, make the local variables for the arguments
		//TODO: figure out if this is correct
		for(DragonDeclaredArgumentNode arg: methodNode.arguments){
		    methodScopeSymbolTable.add(new DragonMethodScopeVariableSymbolTableRow(arg.name.name,arg.type.typeName));
		}

		for(DragonStatementNode stmt : methodNode.statements) {

		    //TODO: also get the assignment statements recursively
		    //that are inside the control flow statements.
		    //but that is for later

		    if(stmt.statementNode instanceof DragonAssignmentStatementNode) {
		        DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode)stmt.statementNode;

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
