package org.vanautrui.languages.compiler.typeresolution;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.CharConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.FloatConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.Arrays;
import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.TypeChecker.isIntegralType;

public final class TypeResolver {

    //prevents instance creation
    private TypeResolver(){}

    public static IBasicAndWrappedTypeNode getTypeIntegerConstantNode(final IntConstNode intConstNode) throws Exception {
        if(intConstNode.number >= 0){
            return new SimpleTypeNode("PInt");
        }else{
            return new SimpleTypeNode("NInt");
        }
    }

    public static IBasicAndWrappedTypeNode getTypeFloatConstantNode() throws Exception {
        //TODO: decide between NZFloat, PFloat, ...
    	return new SimpleTypeNode("Float");
    }

    public static TypeNode getTypeVariableNode(
            final VariableNode varNode,
            final SymbolTableContext ctx
    )throws Exception{
        final SubroutineSymbolTable subTable = ctx.subTable;
        final LocalVarSymbolTable varTable = ctx.varTable;
        final StructsSymbolTable structsTable = ctx.structsTable;

        //a variable can have any type, maybe a subroutine type, a type variable type, or a simple type or maybe even something else

        //if it is a struct, we must get the last type
        //in the chain of member access operations

        if( varTable.containsVariable(varNode.simpleVariableNode.name) ) {
            //the symbol table should contain the array type, if it is an array
            //but this method should return the type also if it has an index


            var type=varTable.getTypeOfVariable(varNode.simpleVariableNode.name);

            if (varNode.simpleVariableNode.indexOptional.isPresent()) {

                type= new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode(type.getTypeName().substring(1, type.getTypeName().length() - 1))));
            }

            //now check for member access
            if(varNode.memberAccessList.size()>0){
                //so it is a struct. walk along the membe accesses to find the type at the end
                int i=0;
                while(i<varNode.memberAccessList.size()){
                    final StructsSymbolTableRow row = structsTable.get(type.getTypeName());

                    String typeOfMember = row.getTypeOfMember(varNode.memberAccessList.get(i).simpleVariableNode.name);
                    if(varNode.memberAccessList.get(i).simpleVariableNode.indexOptional.isPresent()){
                        typeOfMember=typeOfMember.substring(1,typeOfMember.length()-1);
                    }

                    type=new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode(typeOfMember)));
                    i++;
                }
            }

            return type;

        }else if(subTable.containsSubroutine(varNode.simpleVariableNode.name)){

            return subTable.getTypeOfSubroutine(varNode.simpleVariableNode.name);
        }else{

            throw new Exception("could not determine type of "+varNode.toSourceCode());
        }
    }

    public static TypeNode getTypeTermNode(
            final TermNode termNode,
            final SymbolTableContext ctx
    )throws Exception{

        final SubroutineSymbolTable subTable = ctx.subTable;
        final LocalVarSymbolTable varTable = ctx.varTable;
        final StructsSymbolTable structsTable = ctx.structsTable;

        if(termNode.termNode instanceof ExpressionNode){
            return getTypeExpressionNode((ExpressionNode)termNode.termNode,ctx);
        }else if (termNode.termNode instanceof MethodCallNode){
            return getTypeMethodCallNode((MethodCallNode)termNode.termNode,subTable,varTable);
	      }else if(termNode.termNode instanceof FloatConstNode){
		        return new TypeNode(new BasicTypeWrappedNode(getTypeFloatConstantNode()));
        }else if(termNode.termNode instanceof IntConstNode){
            return new TypeNode(new BasicTypeWrappedNode(getTypeIntegerConstantNode((IntConstNode)termNode.termNode)));
        }else if(termNode.termNode instanceof VariableNode){
            return getTypeVariableNode((VariableNode)termNode.termNode,ctx);
		    }else if(termNode.termNode instanceof BoolConstNode) {
            return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Bool")));
        }else if(termNode.termNode instanceof ArrayConstantNode) {
            return new TypeNode(new BasicTypeWrappedNode(getTypeArrayConstNode((ArrayConstantNode) termNode.termNode, ctx)));
        }else if(termNode.termNode instanceof CharConstNode){
            return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Char")));
        }else{
            throw new Exception("unforeseen case in getTypeTermNode(...) in DragonTypeResolver");
        }

    }

    private static IBasicAndWrappedTypeNode getTypeArrayConstNode(
            final ArrayConstantNode arrayConstantNode,
            final SymbolTableContext ctx
    ) throws Exception {
        final SubroutineSymbolTable subTable = ctx.subTable;
        final LocalVarSymbolTable varTable = ctx.varTable;
        final StructsSymbolTable structsTable = ctx.structsTable;

        //since the array types should be all the same,
        //that should be checked in the package responsible for typechecking
        //here we assume it will be checked there

        if(arrayConstantNode.elements.size()==0){
            throw new Exception("array size should be atleast 1, for the type to be inferred without type annotations: "+arrayConstantNode.toSourceCode()+" , ");
        }

        //for the array to have a type, it has to either be annotated,
        // or contain atleast 1 element of which the type can be known
        return new SimpleTypeNode("["+getTypeExpressionNode(arrayConstantNode.elements.get(0),ctx).getTypeName()+"]");
    }



    public static TypeNode getTypeExpressionNode(
            final ExpressionNode expressionNode,
            final SymbolTableContext ctx
    ) throws Exception {
        final SubroutineSymbolTable subTable = ctx.subTable;
        final LocalVarSymbolTable varTable = ctx.varTable;
        final StructsSymbolTable structsTable = ctx.structsTable;

        final List<String> boolean_operators = Arrays.asList("<",">","<=",">=","==","!=");
        final List<String> primitive_types_not_integral = Arrays.asList("Bool","Char","Float");

        if(
                isIntegralType(getTypeTermNode(expressionNode.term,ctx)) &&
                        expressionNode.termNodes.size()==1 &&
                        isIntegralType(getTypeTermNode(expressionNode.termNodes.get(0),ctx)) &&
                        expressionNode.operatorNodes.size()==1 &&
                        (boolean_operators.contains(expressionNode.operatorNodes.get(0).operator))
        ){
            return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode( "Bool")));
        }

        for(final String primitive_type_not_integral : primitive_types_not_integral) {
            if (
                    getTypeTermNode(expressionNode.term, ctx).getTypeName().equals(primitive_type_not_integral) &&
                            expressionNode.termNodes.size() == 1 &&
                            getTypeTermNode(expressionNode.termNodes.get(0), ctx).getTypeName().equals(primitive_type_not_integral) &&
                            expressionNode.operatorNodes.size() == 1 &&
                            (boolean_operators.contains(expressionNode.operatorNodes.get(0).operator))
            ) {
                return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Bool")));
            }
        }

        return getTypeExpressionNodeNonSimple(expressionNode, ctx);
    }

    private static TypeNode getTypeExpressionNodeNonSimple(
            final ExpressionNode expressionNode,
            final SymbolTableContext ctx
    ) throws Exception {
        final SubroutineSymbolTable subTable = ctx.subTable;
        final LocalVarSymbolTable varTable = ctx.varTable;
        final StructsSymbolTable structsTable = ctx.structsTable;

        final List<String> some_arithmetic_operators = Arrays.asList("+","-","*","/","%");

        final TypeNode type = getTypeTermNode(expressionNode.term,ctx);

        for (final TermNode t : expressionNode.termNodes){
            TypeNode termType = getTypeTermNode(t,ctx);

            if(!(termType.getTypeName().equals(type.getTypeName()))){

                if(
                        isIntegralType(getTypeTermNode(expressionNode.term,ctx)) &&
                                expressionNode.termNodes.size()==1 &&
                                isIntegralType(getTypeTermNode(expressionNode.termNodes.get(0),ctx)) &&
                                expressionNode.operatorNodes.size()==1 &&
                                (some_arithmetic_operators.contains(expressionNode.operatorNodes.get(0).operator))
                ){
                    return new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode( "Int")));
                }

                throw new Exception(
                        "the types are not the same, "+type.getTypeName()+" collides with "+termType.getTypeName()
                                +" in '"+expressionNode.toSourceCode()+"'"
                );
            }
        }

        return getTypeTermNode(expressionNode.term,ctx);
    }

    public static TypeNode getTypeMethodCallNode(
            final MethodCallNode methodCallNode,
            final SubroutineSymbolTable subTable,
            final LocalVarSymbolTable varTable
    ) throws Exception
    {

        final String subrName = methodCallNode.methodName;

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
