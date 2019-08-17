package org.vanautrui.languages.typechecking;

import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonDeclaredArgumentNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonOperatorNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonWhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.terminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassFieldNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

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

                typeCheckClassNode(asts,classNode);
            }
        }
    }

    private void typeCheckClassNode(Set<DragonAST> asts, DragonClassNode classNode) throws Exception{
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
            typeCheckMethodNode(asts,classNode,methodNode);
        }

        if(count!=1){
            throw new Exception("multiple definitions of class '"+classNode.name+"'");
        }
    }

    private void typeCheckMethodNode(Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode) throws Exception{

        typeCheckMethodNameNode(asts,classNode,methodNode.methodName);

        typeCheckTypeIdentifierNode(asts,classNode,methodNode.type);
        for(DragonStatementNode stmt : methodNode.statements){
            //stmt.doTypeCheck(asts,classNode, Optional.of(methodNode));
            typeCheckStatementNode(asts,classNode,methodNode,stmt);
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

    private void typeCheckStatementNode(Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonStatementNode statementNode)throws Exception{
        //TODO: it depends on the instance
        if(statementNode.statementNode instanceof DragonAssignmentStatementNode) {
            DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode) statementNode.statementNode;
            typeCheckAssignmentStatementNode(asts, classNode, methodNode, assignmentStatementNode);
        }else if(statementNode.statementNode instanceof DragonLoopStatementNode){
            DragonLoopStatementNode loopStatementNode = (DragonLoopStatementNode) statementNode.statementNode;
            typeCheckLoopStatementNode(asts, classNode, methodNode, loopStatementNode);
        }else if(statementNode.statementNode instanceof DragonWhileStatementNode){
            DragonWhileStatementNode whileStatementNode = (DragonWhileStatementNode) statementNode.statementNode;
            typeCheckWhileStatementNode(asts, classNode, methodNode, whileStatementNode);
        }else if(statementNode.statementNode instanceof DragonMethodCallNode){
            DragonMethodCallNode methodCallNode = (DragonMethodCallNode) statementNode.statementNode;
            typeCheckMethodCallNode(asts, classNode, methodNode, methodCallNode);
        }else{
            throw new Exception("unconsidered case in typechecking ");
        }
    }

    private void typeCheckMethodCallNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonMethodCallNode methodCallNode) throws Exception{
        //TODO: check that the method is called on an object
        //which is actually declared and initialized
        //and is in scope

        //for static method calls, check that the class exists

        //all arguments should typecheck
        for(DragonExpressionNode expr : methodCallNode.argumentList){
            typeCheckExpressionNode(asts,classNode,methodNode,expr);
        }
    }

    private void typeCheckExpressionNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonExpressionNode expr) throws Exception{
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

        for(DragonOperatorNode op : expr.operatorNodes){
            if(!op.operator.equals("+")){
                throw new Exception("only '+' is supported for now");
            }
        }

        for (DragonTermNode t : expr.termNodes){
            if(!(t.termNode instanceof DragonIntegerConstantNode)){
                throw new Exception("only integers are supported for now. the other stuff shall follow later");
            }

            //typecheck the term node, maybe it contains identifiers that are not declared?
            typecheckTermNode(asts,classNode,methodNode,t);
        }

        //TODO: look for the other cases
    }

    private void typecheckTermNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonTermNode termNode) throws Exception{
        if(termNode.termNode instanceof DragonIntegerConstantNode){
            //nothing to do
        }
        if(termNode.termNode instanceof DragonStringConstantNode){
            //nothing to do
        }
        if(termNode.termNode instanceof DragonExpressionNode) {
            DragonExpressionNode expressionNode = (DragonExpressionNode) termNode.termNode;
            typeCheckExpressionNode(asts,classNode,methodNode,expressionNode);
        }else if(termNode.termNode instanceof DragonVariableNode){
            DragonVariableNode variableNode = (DragonVariableNode) termNode.termNode;
            typeCheckVariableNode(asts,classNode,methodNode,variableNode);
        }else{
            throw new Exception("unhandled case");
        }
    }

    private void typeCheckVariableNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonVariableNode variableNode) throws Exception{
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

        //TODO: search if identifier is declared as a variable
        //by a statement above its usage in the method
        //this should maybe be done in another implementation
        //of this method in another node class,
        //perhaps in MethodNode.doTypeCheck();

        throw new Exception("could not find declaration for usage of variable '"+variableNode.name.getContents()+"'");
    }

    private void typeCheckWhileStatementNode(Set<DragonAST> asts, DragonClassNode classNode, DragonMethodNode methodNode, DragonWhileStatementNode whileStatementNode) throws Exception{
        //the condition expression should be of type boolean
        if(!whileStatementNode.condition.getType(methodNode).equals("Bool") && !whileStatementNode.condition.getType(methodNode).equals("Boolean")){
            throw new Exception(" condition should be of type boolean");
        }
        for(DragonStatementNode stmt : whileStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt);
        }
    }

    private void typeCheckLoopStatementNode(
            Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonLoopStatementNode loopStatementNode
    ) throws Exception{
        //the condition expression should be of type boolean
        if(!loopStatementNode.count.getType(methodNode).equals("Int")){
            throw new Exception(" condition should be of type Int . this is a loop statement after all.");
        }
        for(DragonStatementNode stmt : loopStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt);
        }
    }

    private void typeCheckAssignmentStatementNode(
            Set<DragonAST> asts, DragonClassNode classNode,DragonMethodNode methodNode,DragonAssignmentStatementNode assignmentStatementNode
    ) throws Exception{
        if(!assignmentStatementNode.variableNode.getType(methodNode).equals(assignmentStatementNode.expressionNode.getType(methodNode))){
            throw new Exception("with an assignment, both sides have to have the same type. here, a value of type "+assignmentStatementNode.expressionNode.getType(methodNode)+" was assigned to a value of type "+assignmentStatementNode.variableNode.getType(methodNode));
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

        if(typeIdentifierNode.typeName.getContents().equals("Void")){
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
