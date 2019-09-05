package org.vanautrui.languages.typechecking;

import org.apache.commons.lang3.StringUtils;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.symboltablegenerator.*;
import org.vanautrui.languages.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.*;
import org.vanautrui.languages.parsing.astnodes.terminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.*;
import org.vanautrui.languages.typeresolution.TypeResolver;

import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class TypeChecker {

    //this class is supposed to typecheck the program.
    //it receives the 'environment' of a node as arguments
    //so each node can check if it is correct,
    //i.e. that it matches the type expected by its environment
    //and that itself contains only
    //AST Nodes that conform to the expected types.

    public void doTypeCheck(List<AST> asts) throws Exception{
        SubroutineSymbolTable subroutineSymbolTable = SymbolTableGenerator.createSubroutineSymbolTable(new HashSet<>(asts));
        for(AST ast : asts){
            for(ClassNode classNode : ast.classNodeList){
                typeCheckClassNode(asts,classNode,subroutineSymbolTable);
            }
        }
    }

    private void typeCheckClassNode(List<AST> asts, ClassNode classNode, SubroutineSymbolTable subroutineSymbolTable) throws Exception{
        int count=0;
        for(AST ast : asts){
            for(ClassNode dragonClassNode : ast.classNodeList){
                if(dragonClassNode.name.typeName.equals(classNode.name.typeName)){
                    count++;
                }
            }
        }

        //check that fields and methods are typesafe

        for(ClassFieldNode fieldNode : classNode.fieldNodeList){
            typeCheckClassFieldNode(asts,classNode,fieldNode);
        }

        for(MethodNode methodNode : classNode.methodNodeList){
            typeCheckMethodNode(asts,classNode,methodNode,subroutineSymbolTable);
        }

        if(count!=1){
            throw new Exception("multiple definitions of class '"+classNode.name.typeName+"'");
        }
    }

    private void typeCheckMethodNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, SubroutineSymbolTable subTable) throws Exception{

        //create the variable Symbol table, to typecheck the statements
        LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(methodNode,subTable);

        typeCheckMethodNameNode(asts,classNode,methodNode.methodName);

        typeCheckTypeIdentifierNode(asts,classNode,methodNode.type);
        for(StatementNode stmt : methodNode.statements){
            //stmt.doTypeCheck(asts,classNode, Optional.of(methodNode));
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
        for(DeclaredArgumentNode arg : methodNode.arguments){
            typeCheckDeclaredArgumentNode(asts,classNode,arg);
        }

        //at the end of every method/function, there has to be some return statement
        //atleast for now

        StatementNode statementNode = methodNode.statements.get(methodNode.statements.size() - 1);
        if(! (statementNode.statementNode instanceof ReturnStatementNode)){

            throw new Exception("error in typechecking : "+methodNode.methodName.methodName.name+" does not have a return statement as the last statement ");
        }
    }

    private void typeCheckClassFieldNode(
            List<AST> asts, ClassNode classNode, ClassFieldNode classFieldNode
    ) throws Exception{
        typeCheckTypeIdentifierNode(asts,classNode,classFieldNode.type);
    }

    private void typeCheckStatementNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, StatementNode statementNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable)throws Exception{
        //it depends on the instance
        if(statementNode.statementNode instanceof AssignmentStatementNode) {
            AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) statementNode.statementNode;
            typeCheckAssignmentStatementNode(asts, classNode, methodNode, assignmentStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof LoopStatementNode){
            LoopStatementNode loopStatementNode = (LoopStatementNode) statementNode.statementNode;
            typeCheckLoopStatementNode(asts, classNode, methodNode, loopStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof WhileStatementNode){
            WhileStatementNode whileStatementNode = (WhileStatementNode) statementNode.statementNode;
            typeCheckWhileStatementNode(asts, classNode, methodNode, whileStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof MethodCallNode) {
            MethodCallNode methodCallNode = (MethodCallNode) statementNode.statementNode;
            typeCheckMethodCallNode(asts, classNode, methodNode, methodCallNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof IfStatementNode) {
            IfStatementNode ifStatementNode = (IfStatementNode) statementNode.statementNode;
            typeCheckIfStatementNode(asts, classNode, methodNode, ifStatementNode,subTable,varTable);
        }else if(statementNode.statementNode instanceof ReturnStatementNode){
            ReturnStatementNode returnStatementNode = (ReturnStatementNode)statementNode.statementNode;
            typeCheckReturnStatementNode(asts,classNode,methodNode,returnStatementNode,subTable,varTable);
        }else{
            throw new Exception("unconsidered case in typechecking ");
        }
    }

    private void typeCheckReturnStatementNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, ReturnStatementNode returnStatementNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
        //well the type of the value returned should be the same as the method return type
        //in case of void there should be no value returned
        {
            String returnValueType= TypeResolver.getTypeExpressionNode(returnStatementNode.returnValue,methodNode,subTable,varTable);
            if(
                !(returnValueType.equals(methodNode.type.typeName))
            ){
                throw new Exception(" return type has to equal the method type");
            }
		typeCheckExpressionNode(asts,classNode,methodNode,returnStatementNode.returnValue,subTable,varTable);

        }
    }

    private void typeCheckIfStatementNode(List<AST> asts, ClassNode classNode,
                                          MethodNode methodNode,
                                          IfStatementNode ifStatementNode,
                                          SubroutineSymbolTable subTable,
                                          LocalVarSymbolTable varTable) throws Exception{
        //the condition expression should be of type boolean
        String conditionType = TypeResolver.getTypeExpressionNode(ifStatementNode.condition,methodNode,subTable,varTable);
        if(!conditionType.equals("Bool") && !conditionType.equals("Boolean")){
            throw new Exception(" condition should be of type boolean");
        }
        for(StatementNode stmt : ifStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
        for(StatementNode stmt : ifStatementNode.elseStatements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
    }

    private void typeCheckMethodCallNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, MethodCallNode methodCallNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
        //TODO: check that the method is called on an object
        //which is actually declared and initialized
        //and is in scope

		if(!subTable.containsSubroutine(methodCallNode.identifierMethodName)){
            List<String> builtins = Arrays.asList("print","println","read","readln");
            if(!builtins.contains(methodCallNode.identifierMethodName)){
			     System.out.println(subTable.toString());
			     throw new Exception("name of method not in subroutine symbol table: "+methodCallNode.identifierMethodName);
            }
		}

        //for static method calls, check that the class exists

        //all arguments should typecheck
        for(ExpressionNode expr : methodCallNode.argumentList){
            typeCheckExpressionNode(asts,classNode,methodNode,expr,subTable,varTable);
        }
    }

    private void typeCheckExpressionNode(
            List<AST> asts, ClassNode classNode, MethodNode methodNode,
            ExpressionNode expr, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable
    ) throws Exception{
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
	String type= TypeResolver.getTypeTermNode(expr.term,methodNode,subTable,varTable);
	List<String> currentAllowedTypes=Arrays.asList("Int","Float");

	//because the operators on them are not yet defined
	List<String> lonelyAllowedTypes=Arrays.asList("String","[Int]","Char");
	if(!currentAllowedTypes.contains(type)){
	    if(lonelyAllowedTypes.contains(type) && expr.termNodes.size()==0){

			//string may be there as a single expression 
			//to return a string from an subroutine or print one
			//currently we do not support concatenation and such

		    //TODO: make it generic for all array types

            //a single array
        }else{
			throw new Exception(type+" is not in the currently allowed types");
		}
	}
        for (TermNode t : expr.termNodes){
            if( !( TypeResolver.getTypeTermNode(t,methodNode,subTable,varTable).equals(type) ) ){
                throw new Exception("for now, all types in an expression must be the same");
            }
            //typecheck the term node, maybe it contains identifiers that are not declared?
            typecheckTermNode(asts,classNode,methodNode,t,subTable,varTable);
        }

	List<String> currentAllowedOPs=Arrays.asList("+","-","*","/");
        for(OperatorNode op : expr.operatorNodes){
            if(!currentAllowedOPs.contains(op.operator)){
                throw new Exception("currently not supported operator: "+op.operator);
            }
        }	
        //TODO: look for the other cases
    }

    private void typecheckTermNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, TermNode termNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
	if(termNode.termNode instanceof FloatConstNode){
		    //nothing to do
	}else if(termNode.termNode instanceof IntConstNode){
            //nothing to do
        }else if(termNode.termNode instanceof StringConstNode){
            //nothing to do
        }else if(termNode.termNode instanceof ExpressionNode) {
            ExpressionNode expressionNode = (ExpressionNode) termNode.termNode;
            typeCheckExpressionNode(asts,classNode,methodNode,expressionNode,subTable,varTable);
        }else if(termNode.termNode instanceof VariableNode){
            VariableNode variableNode = (VariableNode) termNode.termNode;
            typeCheckVariableNode(asts,classNode,methodNode,variableNode,subTable,varTable);
	}else if(termNode.termNode instanceof MethodCallNode){
		typeCheckMethodCallNode(asts,classNode,methodNode,(MethodCallNode)termNode.termNode,subTable,varTable);
        }else{
            throw new Exception("unhandled case");
        }
    }

    private void typeCheckVariableNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, VariableNode variableNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
        //TODO: it should check that the variable is
        //declared in method scope or class scope.
        //so there should be some declaration of it
        //TODO: also check that the variable is not duplicate declaration

        //identifiers can only be used within a class or method so
        //there should be a context

        for(ClassFieldNode fieldNode : classNode.fieldNodeList){
            if(fieldNode.name.name.equals(variableNode.name)){
                //found the identifier declared here
                return;
            }
        }

        //it is not declared in class scope, it should be declared in
        //method scope

        for(DeclaredArgumentNode arg : methodNode.arguments){
            if(arg.name.name.equals(variableNode.name)){
                return;
            }
        }

        //search if identifier is declared as a variable
	if(varTable.containsVariable(variableNode.name)){
		return;
	}

        throw new Exception("could not find declaration for usage of variable '"+variableNode.name+"' \n"+subTable.toString());
    }

    private void typeCheckWhileStatementNode(
            List<AST> asts,
            ClassNode classNode,
            MethodNode methodNode,
         WhileStatementNode whileStatementNode,
         SubroutineSymbolTable subTable,
         LocalVarSymbolTable varTable
    ) throws Exception{
        //the condition expression should be of type boolean
        String conditionType= TypeResolver.getTypeExpressionNode(whileStatementNode.condition,methodNode,subTable,varTable);
        if(!conditionType.equals("Bool")){
            throw new Exception(" condition should be of type Bool : '"+whileStatementNode.condition.toSourceCode()+"' but was of type: "+conditionType);
        }
        for(StatementNode stmt : whileStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
    }

    private void typeCheckLoopStatementNode(
            List<AST> asts, ClassNode classNode, MethodNode methodNode,
            LoopStatementNode loopStatementNode, SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable
    ) throws Exception{
        //the condition expression should be of type boolean

        String countType= TypeResolver.getTypeExpressionNode(loopStatementNode.count,methodNode,subTable,varTable);
        if(!countType.equals("Int")){
            throw new Exception(" condition should be of type Int . this is a loop statement after all.");
        }
        for(StatementNode stmt : loopStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
    }

    private void typeCheckAssignmentStatementNode(
            List<AST> asts, ClassNode classNode, MethodNode methodNode,
            AssignmentStatementNode assignmentStatementNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable
    ) throws Exception{
        String leftSideType = TypeResolver.getTypeVariableNode(assignmentStatementNode.variableNode,methodNode,subTable,varTable);
        String rightSideType = TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,varTable);
        if(!leftSideType.equals(rightSideType)){
            throw new Exception(
                    "with an assignment, both sides have to have the same type. here, a value of type "+rightSideType+" was assigned to a value of type "+leftSideType
                    +" in source: '"+assignmentStatementNode.toSourceCode()+"'"
            );
        }
    }

    private void typeCheckTypeIdentifierNode(
            List<AST> asts, ClassNode classNode,
            TypeIdentifierNode typeIdentifierNode
            ) throws Exception{

        //check that the type is defined somewhere
        //so there should exist a class with that type

        for(AST ast : asts){
            for(ClassNode myclassNode : ast.classNodeList){
                if(myclassNode.name.typeName.equals(typeIdentifierNode.typeName)){
                    return;
                }
            }
        }

        List<String> acceptable_types = Arrays.asList("Int","Float","Bool","String","[Int]","[Float]","[Bool]","[String]");

        if(acceptable_types.contains(typeIdentifierNode.typeName)){
            return;
        }

        String msg = TerminalUtil.gererateErrorString("TYPECHECKING: ")
                + "could not find class for type:"
                + StringUtils.wrap(typeIdentifierNode.typeName,"'")
                +" in file : (TODO: display file and line number)";
        throw new Exception(msg);
    }

    private void typeCheckMethodNameNode(List<AST> asts, ClassNode classNode, MethodNameNode methodNameNode)throws Exception{
        //method names should not be duplicate in a class
        //this may change in another version of dragon

        long count = classNode.methodNodeList.stream().filter(mNode -> mNode.methodName.methodName.name.equals(methodNameNode.methodName.name)).count();

        if(count>1){
            throw new Exception("duplicate declaration of method '"+methodNameNode.methodName.name+"' ");
        }
    }

    private void typeCheckDeclaredArgumentNode(List<AST> asts, ClassNode classNode, DeclaredArgumentNode declaredArgumentNode)throws Exception{
        typeCheckTypeIdentifierNode(asts,classNode,declaredArgumentNode.type);
    }

    private void typeCheckIdentifierNode(Set<AST> asts, ClassNode classNode, MethodNode methodNode, IdentifierNode identifierNode)throws Exception{
        //TODO: it should check that the identifier is
        //declared in method scope or class scope.
        //so there should be some declaration of it
        //TODO: also check that the identifier is not duplicate declaration

        //identifiers can only be used within a class or method so
        //there should be a context


        for(ClassFieldNode fieldNode : classNode.fieldNodeList){
            if(fieldNode.name.name.equals(identifierNode.name)){
                //found the identifier declared here
                return;
            }
        }

        //it is not declared in class scope, it should be declared in
        //method scope

        for(DeclaredArgumentNode arg : methodNode.arguments){
            if(arg.name.name.equals(identifierNode.name)){
                return;
            }
        }

        //TODO: search if identifier is declared as a variable
        //by a statement above its usage in the method
        //this should maybe be done in another implementation
        //of this method in another node class,
        //perhaps in MethodNode.doTypeCheck();

        throw new Exception("could not find declaration for usage of Identifier '"+identifierNode.name+"'");
    }
}
