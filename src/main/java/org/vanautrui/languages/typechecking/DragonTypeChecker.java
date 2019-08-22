package org.vanautrui.languages.typechecking;

import org.vanautrui.languages.symboltablegenerator.*;
import org.vanautrui.languages.codegeneration.*;
import org.vanautrui.languages.codegeneration.symboltables.tables.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.*;
import org.vanautrui.languages.parsing.astnodes.terminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.*;
import org.vanautrui.languages.typeresolution.DragonTypeResolver;

import java.util.Arrays;
import java.util.List;
import java.util.Set;

public class DragonTypeChecker {

    //this class is supposed to typecheck the program.
    //it receives the 'environment' of a node as arguments
    //so each node can check if it is correct,
    //i.e. that it matches the type expected by its environment
    //and that itself contains only
    //AST Nodes that conform to the expected types.

    public void doTypeCheck(Set<DragonAST> asts) throws Exception{

        for(DragonAST ast : asts){
            for(DragonClassNode classNode : ast.classNodeList){

                //TODO: create a the  symbol table with all classes being compiled
                //otherwise we cannot call methods from other classes
                DragonSubroutineSymbolTable subroutineSymbolTable = DragonSymbolTableGenerator.createSubroutineSymbolTable(classNode);

                typeCheckClassNode(asts,classNode,subroutineSymbolTable);
            }
        }
    }

    private void typeCheckClassNode(Set<DragonAST> asts, DragonClassNode classNode,DragonSubroutineSymbolTable subroutineSymbolTable) throws Exception{
        int count=0;
        for(DragonAST ast : asts){
            for(DragonClassNode dragonClassNode : ast.classNodeList){
                if(dragonClassNode.name.typeName.getContents().equals(classNode.name.typeName.getContents())){
                    count++;
                }
            }
        }

        //check that fields and methods are typesafe

        for(DragonClassFieldNode fieldNode : classNode.fieldNodeList){
            typeCheckClassFieldNode(asts,classNode,fieldNode);
        }

        for(DragonMethodNode methodNode : classNode.methodNodeList){
            typeCheckMethodNode(asts,classNode,methodNode,subroutineSymbolTable);
        }

        if(count!=1){
            throw new Exception("multiple definitions of class '"+classNode.name+"'");
        }
    }

    private void typeCheckMethodNode(Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonSubroutineSymbolTable subTable) throws Exception{

	//create the variable Symbol table, to typecheck the statements
	DragonMethodScopeVariableSymbolTable varTable = DragonSymbolTableGenerator.createMethodScopeSymbolTable(methodNode,subTable);

        typeCheckMethodNameNode(asts,classNode,methodNode.methodName);

        typeCheckTypeIdentifierNode(asts,classNode,methodNode.type);
        for(DragonStatementNode stmt : methodNode.statements){
            //stmt.doTypeCheck(asts,classNode, Optional.of(methodNode));
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
        for(DragonDeclaredArgumentNode arg : methodNode.arguments){
            typeCheckDeclaredArgumentNode(asts,classNode,arg);
        }
    }

    private void typeCheckClassFieldNode(
            Set<DragonAST> asts, DragonClassNode classNode,DragonClassFieldNode classFieldNode
    ) throws Exception{
        typeCheckTypeIdentifierNode(asts,classNode,classFieldNode.type);
    }

    private void typeCheckStatementNode(Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonStatementNode statementNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable)throws Exception{
        //it depends on the instance
        if(statementNode.statementNode instanceof DragonAssignmentStatementNode) {
            DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode) statementNode.statementNode;
            typeCheckAssignmentStatementNode(asts, classNode, methodNode, assignmentStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof DragonLoopStatementNode){
            DragonLoopStatementNode loopStatementNode = (DragonLoopStatementNode) statementNode.statementNode;
            typeCheckLoopStatementNode(asts, classNode, methodNode, loopStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof DragonWhileStatementNode){
            DragonWhileStatementNode whileStatementNode = (DragonWhileStatementNode) statementNode.statementNode;
            typeCheckWhileStatementNode(asts, classNode, methodNode, whileStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof DragonMethodCallNode) {
            DragonMethodCallNode methodCallNode = (DragonMethodCallNode) statementNode.statementNode;
            typeCheckMethodCallNode(asts, classNode, methodNode, methodCallNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof DragonIfStatementNode) {
            DragonIfStatementNode ifStatementNode = (DragonIfStatementNode) statementNode.statementNode;
            typeCheckIfStatementNode(asts, classNode, methodNode, ifStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof DragonReturnStatementNode){
            DragonReturnStatementNode returnStatementNode = (DragonReturnStatementNode)statementNode.statementNode;
            typeCheckReturnStatementNode(asts,classNode,methodNode,returnStatementNode,subTable,varTable);
        }else{
            throw new Exception("unconsidered case in typechecking ");
        }
    }

    private void typeCheckReturnStatementNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonReturnStatementNode returnStatementNode, DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
        //well the type of the value returned should be the same as the method return type
        //in case of void there should be no value returned
        if(methodNode.type.typeName.getContents().equals("Void")){
            if(returnStatementNode.returnValue.isPresent()){
                throw new Exception(" Type Checking Failed. do not return a value from a Void method. "+returnStatementNode.returnValue.get().toSourceCode());
            }
        }else{
            String returnValueType= DragonTypeResolver.getTypeExpressionNode(returnStatementNode.returnValue.get(),methodNode,subTable,varTable);
            if(
                !(returnValueType.equals(methodNode.type.typeName.getContents()))
            ){
                throw new Exception(" return type has to equal the method type");
            }
		typeCheckExpressionNode(asts,classNode,methodNode,returnStatementNode.returnValue.get(),subTable,varTable);

        }
    }

    private void typeCheckIfStatementNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonIfStatementNode ifStatementNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
        //the condition expression should be of type boolean
        String conditionType = DragonTypeResolver.getTypeExpressionNode(ifStatementNode.condition,methodNode,subTable,varTable);
        if(!conditionType.equals("Bool") && !conditionType.equals("Boolean")){
            throw new Exception(" condition should be of type boolean");
        }
        for(DragonStatementNode stmt : ifStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
        for(DragonStatementNode stmt : ifStatementNode.elseStatements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
    }

    private void typeCheckMethodCallNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonMethodCallNode methodCallNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
        //TODO: check that the method is called on an object
        //which is actually declared and initialized
        //and is in scope

        //for static method calls, check that the class exists

        //all arguments should typecheck
        for(DragonExpressionNode expr : methodCallNode.argumentList){
            typeCheckExpressionNode(asts,classNode,methodNode,expr,subTable,varTable);
        }
    }

    private void typeCheckExpressionNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonExpressionNode expr,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
        //check that the terms have a type such that the operator will work

        //for later:
        //int * string      //repeat the string x times

        //string + string   //concatenate
        //int + int
        //int - int
        //int * int
        //int / int

        //expression is compiled to evaluate in order. meaning
        //that if there are 3 terms,
        //the first gets evaluated, then the second, then the operator gets applied
        //such for convienience we check for now that the terms have all the same type

        //as of now i think we should first focus on integer addition
        //and let the other cases throw an exception
        //they should be implemented later

	//the types should be all the same for now
	typecheckTermNode(asts,classNode,methodNode,expr.term,subTable,varTable);
	String type=DragonTypeResolver.getTypeTermNode(expr.term,methodNode,subTable,varTable);
	List<String> currentAllowedTypes=Arrays.asList("Int","Float");
	if(!currentAllowedTypes.contains(type)){
		if(expr.termNodes.size()==0 && type.equals("String")){
			//string may be there as a single expression 
			//to return a string from an subroutine or print one
			//currently we do not support concatenation and such
		}else{
			throw new Exception(type+" is not in the currently allowed types");
		}
	}
        for (DragonTermNode t : expr.termNodes){
            if( !( DragonTypeResolver.getTypeTermNode(t,methodNode,subTable,varTable).equals(type) ) ){
                throw new Exception("for now, all types in an expression must be the same");
            }
            //typecheck the term node, maybe it contains identifiers that are not declared?
            typecheckTermNode(asts,classNode,methodNode,t,subTable,varTable);
        }

	List<String> currentAllowedOPs=Arrays.asList("+","-","*","/");
        for(DragonOperatorNode op : expr.operatorNodes){
            if(!currentAllowedOPs.contains(op.operator)){
                throw new Exception("currently not supported operator: "+op.operator);
            }
        }	
        //TODO: look for the other cases
    }

    private void typecheckTermNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonTermNode termNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
	if(termNode.termNode instanceof DragonFloatConstantNode){
		    //nothing to do
	}else if(termNode.termNode instanceof DragonIntegerConstantNode){
            //nothing to do
        }else if(termNode.termNode instanceof DragonStringConstantNode){
            //nothing to do
        }else if(termNode.termNode instanceof DragonExpressionNode) {
            DragonExpressionNode expressionNode = (DragonExpressionNode) termNode.termNode;
            typeCheckExpressionNode(asts,classNode,methodNode,expressionNode,subTable,varTable);
        }else if(termNode.termNode instanceof DragonVariableNode){
            DragonVariableNode variableNode = (DragonVariableNode) termNode.termNode;
            typeCheckVariableNode(asts,classNode,methodNode,variableNode,subTable,varTable);
	}else if(termNode.termNode instanceof DragonMethodCallNode){
		typeCheckMethodCallNode(asts,classNode,methodNode,(DragonMethodCallNode)termNode.termNode,subTable,varTable);
        }else{
            throw new Exception("unhandled case");
        }
    }

    private void typeCheckVariableNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonVariableNode variableNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
        //TODO: it should check that the variable is
        //declared in method scope or class scope.
        //so there should be some declaration of it
        //TODO: also check that the variable is not duplicate declaration

        //identifiers can only be used within a class or method so
        //there should be a context

        for(DragonClassFieldNode fieldNode : classNode.fieldNodeList){
            if(fieldNode.name.name.equals(variableNode.name.getContents())){
                //found the identifier declared here
                return;
            }
        }

        //it is not declared in class scope, it should be declared in
        //method scope

        for(DragonDeclaredArgumentNode arg : methodNode.arguments){
            if(arg.name.name.equals(variableNode.name.getContents())){
                return;
            }
        }

        //search if identifier is declared as a variable
	if(varTable.containsVariable(variableNode.name.getContents())){
		return;
	}

        throw new Exception("could not find declaration for usage of variable '"+variableNode.name.getContents()+"' \n"+subTable.toString());
    }

    private void typeCheckWhileStatementNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonWhileStatementNode whileStatementNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable) throws Exception{
        //the condition expression should be of type boolean
        String conditionType=DragonTypeResolver.getTypeExpressionNode(whileStatementNode.condition,methodNode,subTable,varTable);
        if(!conditionType.equals("Bool") && !conditionType.equals("Boolean")){
            throw new Exception(" condition should be of type boolean");
        }
        for(DragonStatementNode stmt : whileStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
    }

    private void typeCheckLoopStatementNode(
            Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonLoopStatementNode loopStatementNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable
    ) throws Exception{
        //the condition expression should be of type boolean

        String countType= DragonTypeResolver.getTypeExpressionNode(loopStatementNode.count,methodNode,subTable,varTable);
        if(!countType.equals("Int")){
            throw new Exception(" condition should be of type Int . this is a loop statement after all.");
        }
        for(DragonStatementNode stmt : loopStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
    }

    private void typeCheckAssignmentStatementNode(
            Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonAssignmentStatementNode assignmentStatementNode,DragonSubroutineSymbolTable subTable,DragonMethodScopeVariableSymbolTable varTable
    ) throws Exception{
        String leftSideType = DragonTypeResolver.getTypeVariableNode(assignmentStatementNode.variableNode,methodNode,subTable,varTable);
        String rightSideType = DragonTypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,varTable);
        if(!leftSideType.equals(rightSideType)){
            throw new Exception("with an assignment, both sides have to have the same type. here, a value of type "+rightSideType+" was assigned to a value of type "+leftSideType);
        }
    }

    private void typeCheckTypeIdentifierNode(
            Set<DragonAST> asts, DragonClassNode classNode, DragonTypeIdentifierNode typeIdentifierNode
            ) throws Exception{

        //check that the type is defined somewhere
        //so there should exist a class with that type

        for(DragonAST ast : asts){
            for(DragonClassNode myclassNode : ast.classNodeList){
                if(myclassNode.name.typeName.getContents().equals(typeIdentifierNode.typeName.getContents())){
                    return;
                }
            }
        }

        List<String> acceptable_types = Arrays.asList("Void","Int","Float","Bool","String");

        if(acceptable_types.contains(typeIdentifierNode.typeName.getContents())){
            return;
        }

        throw new Exception("could not find class for type: '"+typeIdentifierNode.typeName.getContents()+"'");
    }

    private void typeCheckMethodNameNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNameNode methodNameNode)throws Exception{
        //method names should not be duplicate in a class
        //this may change in another version of dragon

        long count = classNode.methodNodeList.stream().filter(mNode -> mNode.methodName.methodName.name.getContents().equals(methodNameNode.methodName.name.getContents())).count();

        if(count>1){
            throw new Exception("duplicate declaration of method '"+methodNameNode.methodName.name.getContents()+"' ");
        }
    }

    private void typeCheckDeclaredArgumentNode(Set<DragonAST> asts, DragonClassNode classNode,DragonDeclaredArgumentNode declaredArgumentNode)throws Exception{
        typeCheckTypeIdentifierNode(asts,classNode,declaredArgumentNode.type);
    }

    private void typeCheckIdentifierNode(Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonIdentifierNode identifierNode)throws Exception{
        //TODO: it should check that the identifier is
        //declared in method scope or class scope.
        //so there should be some declaration of it
        //TODO: also check that the identifier is not duplicate declaration

        //identifiers can only be used within a class or method so
        //there should be a context


        for(DragonClassFieldNode fieldNode : classNode.fieldNodeList){
            if(fieldNode.name.name.equals(identifierNode.name.getContents())){
                //found the identifier declared here
                return;
            }
        }

        //it is not declared in class scope, it should be declared in
        //method scope

        for(DragonDeclaredArgumentNode arg : methodNode.arguments){
            if(arg.name.name.equals(identifierNode.name.getContents())){
                return;
            }
        }

        //TODO: search if identifier is declared as a variable
        //by a statement above its usage in the method
        //this should maybe be done in another implementation
        //of this method in another node class,
        //perhaps in MethodNode.doTypeCheck();

        throw new Exception("could not find declaration for usage of Identifier '"+identifierNode.name.getContents()+"'");
    }
}
