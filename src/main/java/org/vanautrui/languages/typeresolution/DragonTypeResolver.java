package org.vanautrui.languages.typeresolution;

import org.vanautrui.languages.codegeneration.symboltables.tables.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonDeclaredArgumentNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.*;

import java.util.Arrays;
import java.util.List;

public class DragonTypeResolver {

    //todo: make some class or global subroutine
    //that can then convert the type description directly
    //to a jvm internal representation

    public static String getTypeIntegerConstantNode(DragonIntegerConstantNode integerConstantNode){
        return "Int";
    }
    public static String getTypeFloatConstantNode(DragonFloatConstantNode node){
    	return "Float";
    }

    public static String getTypeStringConstantNode(DragonStringConstantNode stringConstantNode){
        return "String";
    }

    public static String getTypeVariableNode(DragonVariableNode variableNode, DragonMethodNode methodNode,DragonSubroutineSymbolTable subroutineSymbolTable,DragonMethodScopeVariableSymbolTable varTable)throws Exception{
        //TODO: implement by looking at the definitions in the AST and such

        if( varTable.containsVariable(variableNode.name.getContents()) ){
			return varTable.getTypeOfVariable(variableNode.name.getContents());
        }else{
            throw new Exception("could not determine type of "+variableNode.name.getContents());
        }
    }

    public static String getTypeTermNode(DragonTermNode termNode,DragonMethodNode methodNode,DragonSubroutineSymbolTable subroutineSymbolTable,DragonMethodScopeVariableSymbolTable varTable)throws Exception{
        if(termNode.termNode instanceof DragonExpressionNode){
            return getTypeExpressionNode((DragonExpressionNode)termNode.termNode,methodNode,subroutineSymbolTable,varTable);
        }else if (termNode.termNode instanceof DragonMethodCallNode){
            return getTypeMethodCallNode((DragonMethodCallNode)termNode.termNode,subroutineSymbolTable);
	}else if(termNode.termNode instanceof DragonFloatConstantNode){
		return getTypeFloatConstantNode((DragonFloatConstantNode)termNode.termNode);
        }else if(termNode.termNode instanceof DragonIntegerConstantNode){
            return getTypeIntegerConstantNode((DragonIntegerConstantNode)termNode.termNode);
        }else if(termNode.termNode instanceof DragonStringConstantNode){
            return getTypeStringConstantNode((DragonStringConstantNode)termNode.termNode);
        }else if(termNode.termNode instanceof DragonVariableNode){
            return getTypeVariableNode((DragonVariableNode) termNode.termNode,methodNode,subroutineSymbolTable,varTable);
		}else if(termNode.termNode instanceof DragonBoolConstantNode){
			return "Bool";
        }else{
            throw new Exception("unforeseen case in getTypeTermNode(...) in DragonTypeResolver");
        }

    }

    public static String getTypeExpressionNode(DragonExpressionNode expressionNode,DragonMethodNode methodNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
        List<String> boolean_operators= Arrays.asList("<",">","<=",">=","==","!=");

        if(
                getTypeTermNode(expressionNode.term,methodNode,subTable,varTable).equals("Int") &&
                        expressionNode.termNodes.size()==1 &&
                        getTypeTermNode(expressionNode.termNodes.get(0),methodNode,subTable,varTable).equals("Int") &&
                        expressionNode.operatorNodes.size()==1 &&
                        (boolean_operators.contains(expressionNode.operatorNodes.get(0).operator))
        ){
            return "Bool";
        }

		if(
                getTypeTermNode(expressionNode.term,methodNode,subTable,varTable).equals("Float") &&
                        expressionNode.termNodes.size()==1 &&
                        getTypeTermNode(expressionNode.termNodes.get(0),methodNode,subTable,varTable).equals("Float") &&
                        expressionNode.operatorNodes.size()==1 &&
                        (boolean_operators.contains(expressionNode.operatorNodes.get(0).operator))
        ){
            return "Bool";
        }


        String type = getTypeTermNode(expressionNode.term,methodNode,subTable,varTable);

        /*
        for(DragonOperatorNode op : this.operatorNodes){
            if(!op.operator.equals("+")){
                throw new Exception("only '+' is supported for now");
            }
        }

         */

        for (DragonTermNode t : expressionNode.termNodes){
            String termType = getTypeTermNode(t,methodNode,subTable,varTable);

            if(!(termType.equals(type))){
                throw new Exception(
					"the types are not the same, "+type+" collides with "+termType
					+" in '"+expressionNode.toSourceCode()+"'"
				);
            }
        }

        return getTypeTermNode(expressionNode.term,methodNode,subTable,varTable);
    }

    public static String getTypeMethodCallNode(DragonMethodCallNode methodCallNode, DragonSubroutineSymbolTable subroutineSymbolTable) throws Exception{

        String subrName = methodCallNode.getMethodName();

        if(subrName.equals("readln")){
            return "String";
        }

        //TODO: handle the other builtin methods

        if(subroutineSymbolTable.containsVariable(subrName)){
            return subroutineSymbolTable.getTypeOfVariable(subrName);
        }

        //TODO: throw exception if not found in symbol table
        return "Void";
    }
}
