package org.vanautrui.languages.compiler.typeresolution;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.Arrays;
import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.TypeChecker.isIntegralType;

public class TypeResolver {

    //todo: make some class or global subroutine
    //that can then convert the type description directly
    //to a jvm internal representation

    public static IBasicAndWrappedTypeNode getTypeIntegerConstantNode(IntConstNode intConstNode) throws Exception {
        if(intConstNode.value>=0){
            return new SimpleTypeNode("PInt");
        }else{
            return new SimpleTypeNode("NInt");
        }
    }
    public static IBasicAndWrappedTypeNode getTypeFloatConstantNode(FloatConstNode node) throws Exception {
    	return new SimpleTypeNode("Float");
    }

    public static TypeNode getTypeVariableNode(VariableNode variableNode, MethodNode methodNode, SubroutineSymbolTable subroutineSymbolTable, LocalVarSymbolTable varTable)throws Exception{
        //a variable can have any type, maybe a subroutine type, a type variable type, or a simple type or maybe even something else

        if( varTable.containsVariable(variableNode.name) ) {
            //the symbol table should contain the array type, if it is an array
            //but this method should return the type also if it has an index
            if (variableNode.indexOptional.isPresent()) {
                var type = varTable.getTypeOfVariable(variableNode.name);
                return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode(type.getTypeName().substring(1, type.getTypeName().length() - 1))));
            }

            return varTable.getTypeOfVariable(variableNode.name);
        }else if(subroutineSymbolTable.containsSubroutine(variableNode.name)){
            return subroutineSymbolTable.getTypeOfSubroutine(variableNode.name);
        }else{
            throw new Exception("could not determine type of "+variableNode.name);
        }
    }

    public static TypeNode getTypeTermNode(TermNode termNode, MethodNode methodNode,
                                                           SubroutineSymbolTable subroutineSymbolTable,
                                                           LocalVarSymbolTable varTable
    )throws Exception{

        if(termNode.termNode instanceof ExpressionNode){
            return getTypeExpressionNode((ExpressionNode)termNode.termNode,methodNode,subroutineSymbolTable,varTable);
        }else if (termNode.termNode instanceof MethodCallNode){
            return getTypeMethodCallNode((MethodCallNode)termNode.termNode,subroutineSymbolTable,varTable);
	      }else if(termNode.termNode instanceof FloatConstNode){
		        return new TypeNode(new BasicTypeWrappedNode(getTypeFloatConstantNode((FloatConstNode)termNode.termNode)));
        }else if(termNode.termNode instanceof IntConstNode){
            return new TypeNode(new BasicTypeWrappedNode(getTypeIntegerConstantNode((IntConstNode)termNode.termNode)));
        }else if(termNode.termNode instanceof VariableNode){
            return getTypeVariableNode((VariableNode) termNode.termNode,methodNode,subroutineSymbolTable,varTable);
		    }else if(termNode.termNode instanceof BoolConstNode) {
            return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Bool")));
        }else if(termNode.termNode instanceof ArrayConstantNode) {
            return new TypeNode(new BasicTypeWrappedNode(getTypeArrayConstNode((ArrayConstantNode) termNode.termNode, methodNode, subroutineSymbolTable, varTable)));
        }else if(termNode.termNode instanceof CharConstNode){
            return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Char")));
        }else{
            throw new Exception("unforeseen case in getTypeTermNode(...) in DragonTypeResolver");
        }

    }

    private static IBasicAndWrappedTypeNode getTypeArrayConstNode(
            ArrayConstantNode arrayConstantNode,
            MethodNode methodNode,
            SubroutineSymbolTable subroutineSymbolTable,
            LocalVarSymbolTable varTable) throws Exception
    {
        //since the array types should be all the same,
        //that should be checked in the package responsible for typechecking
        //here we assume it will be checked there

        if(arrayConstantNode.elements.size()==0){
            throw new Exception("array size should be atleast 1, for the type to be inferred without type annotations");
        }

        //for the array to have a type, it has to either be annotated,
        // or contain atleast 1 element of which the type can be known
        return new SimpleTypeNode("["+getTypeExpressionNode(arrayConstantNode.elements.get(0),methodNode,subroutineSymbolTable,varTable).getTypeName()+"]");
    }



    public static TypeNode getTypeExpressionNode(
            ExpressionNode expressionNode,
            MethodNode methodNode,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable) throws Exception
    {
        List<String> boolean_operators= Arrays.asList("<",">","<=",">=","==","!=");

        if(
                isIntegralType(getTypeTermNode(expressionNode.term,methodNode,subTable,varTable)) &&
                        expressionNode.termNodes.size()==1 &&
                        isIntegralType(getTypeTermNode(expressionNode.termNodes.get(0),methodNode,subTable,varTable)) &&
                        expressionNode.operatorNodes.size()==1 &&
                        (boolean_operators.contains(expressionNode.operatorNodes.get(0).operator))
        ){
            return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode( "Bool")));
        }

		    if(
                getTypeTermNode(expressionNode.term,methodNode,subTable,varTable).getTypeName().equals("Float") &&
                        expressionNode.termNodes.size()==1 &&
                        getTypeTermNode(expressionNode.termNodes.get(0),methodNode,subTable,varTable).getTypeName().equals("Float") &&
                        expressionNode.operatorNodes.size()==1 &&
                        (boolean_operators.contains(expressionNode.operatorNodes.get(0).operator))
        ){
            return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Bool")));
        }


        TypeNode type = getTypeTermNode(expressionNode.term,methodNode,subTable,varTable);

        for (TermNode t : expressionNode.termNodes){
            TypeNode termType = getTypeTermNode(t,methodNode,subTable,varTable);

            if(!(termType.getTypeName().equals(type.getTypeName()))){
                throw new Exception(
					"the types are not the same, "+type+" collides with "+termType
					+" in '"+expressionNode.toSourceCode()+"'"
				);
            }
        }

        return getTypeTermNode(expressionNode.term,methodNode,subTable,varTable);
    }

    public static TypeNode getTypeMethodCallNode(
            MethodCallNode methodCallNode,
            SubroutineSymbolTable subTable,LocalVarSymbolTable varTable) throws Exception
    {

        String subrName = methodCallNode.methodName;

        if(subTable.containsSubroutine(subrName)){
            return subTable.getReturnTypeOfSubroutine(subrName);
        }

        if(varTable.containsVariable(subrName)){
            return varTable.getReturnTypeOfSubroutineVariable(subrName);
        }

        System.out.println(subTable.toString());
        throw new Exception("could not get type of "+subrName+" (in "+ TypeResolver.class.getSimpleName()+")");
    }
}
