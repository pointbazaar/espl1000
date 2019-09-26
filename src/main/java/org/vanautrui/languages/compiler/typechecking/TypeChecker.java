package org.vanautrui.languages.compiler.typechecking;

import org.apache.commons.lang3.StringUtils;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassFieldNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.simple.SimpleTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.SubroutineTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.Arrays;
import java.util.List;
import java.util.Set;

public class TypeChecker {

    //this class is supposed to typecheck the program.
    //it receives the 'environment' of a node as arguments
    //so each node can check if it is correct,
    //i.e. that it matches the type expected by its environment
    //and that itself contains only
    //AST Nodes that conform to the expected types.



    //the primitive types and their arrays
    public static final List<String> primitive_types_and_arrays_of_them =
            Arrays.asList(
                    "PInt", // Int which is  >= 0
                    "NInt", // Int which is <= 0
                    "Integer",
                    "Float","Bool","Char",
                    "[PInt]","[NInt]",
                    "[Integer]",
                    "[Float]","[Bool]","[Char]"
            );

    public void doTypeCheck(List<AST> asts,boolean debug) throws Exception{
        if(debug){
            System.out.println("TYPECHECKING");
        }
        SubroutineSymbolTable subroutineSymbolTable = SymbolTableGenerator.createSubroutineSymbolTable(asts,debug);
        if(debug){
            System.out.println("generate subroutine symbol table:");
            System.out.println(subroutineSymbolTable.toString());
        }
        for(AST ast : asts){
            for(ClassNode classNode : ast.classNodeList){
                typeCheckClassNode(asts,classNode,subroutineSymbolTable,debug);
            }
        }
    }

    private void typeCheckClassNode(List<AST> asts, ClassNode classNode, SubroutineSymbolTable subroutineSymbolTable,boolean debug) throws Exception{
        if(debug){
            System.out.println("typechecking class:"+classNode.name);
        }
        int count=0;
        for(AST ast : asts){
            for(ClassNode dragonClassNode : ast.classNodeList){
                if(dragonClassNode.name.getTypeName().equals(classNode.name.getTypeName())){
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
            throw new Exception("multiple definitions of class '"+classNode.name.getTypeName()+"'");
        }
    }

    private void typeCheckMethodNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, SubroutineSymbolTable subTable) throws Exception{

        //create the variable Symbol table, to typecheck the statements
        LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(methodNode,subTable);

        typeCheckMethodNameNode(asts,classNode,methodNode.methodName);

        typeCheckTypeIdentifierNode(asts,classNode,methodNode.returnType.typenode);
        for(StatementNode stmt : methodNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
        for(DeclaredArgumentNode arg : methodNode.arguments){
            typeCheckDeclaredArgumentNode(asts,classNode,arg);
        }

        //at the end of every method/function, there has to be some return statement
        //atleast for now

        StatementNode statementNode = methodNode.statements.get(methodNode.statements.size() - 1);
        if(! (statementNode.statementNode instanceof ReturnStatementNode)){

            throw new Exception("error in typechecking : "+methodNode.methodName+" does not have a return statement as the last statement ");
        }
    }

    private void typeCheckClassFieldNode(
            List<AST> asts, ClassNode classNode, ClassFieldNode classFieldNode
    ) throws Exception{
        typeCheckTypeIdentifierNode(asts,classNode,classFieldNode.type.typenode);
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
            ITypeNode returnValueType= TypeResolver.getTypeExpressionNode(returnStatementNode.returnValue,methodNode,subTable,varTable);
            if(
                !(returnValueType.getTypeName().equals(methodNode.returnType.getTypeName()))
            ){
                throw new Exception(this.getClass().getSimpleName()
                        +": return type of the method has to equal the return value type. return type '"
                        +methodNode.returnType.getTypeName()+"' does not equal the returned type '"+returnValueType+"'");
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
        ITypeNode conditionType = TypeResolver.getTypeExpressionNode(ifStatementNode.condition,methodNode,subTable,varTable);
        if(!conditionType.getTypeName().equals("Bool")){
            throw new Exception(" condition should be of type Bool, but is of type: "+conditionType.getTypeName());
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

        boolean found=false;

        if(subTable.containsSubroutine(methodCallNode.methodName)){
            found=true;
		    }

        if(varTable.containsVariable(methodCallNode.methodName) && varTable.get(methodCallNode.methodName).getType() instanceof SubroutineTypeNode){
            found=true;
        }

        if(!found) {
            System.out.println(subTable.toString());
            throw new Exception("name of method not in subroutine symbol table and not in local variable table (or not a subroutine variable): " + methodCallNode.methodName);
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
        ITypeNode type= TypeResolver.getTypeTermNode(expr.term,methodNode,subTable,varTable);
        List<String> currentAllowedTypes=Arrays.asList("PInt","Float");

        if(expr.termNodes.size()==1 && expr.operatorNodes.size()==1 && expr.operatorNodes.get(0).operator.equals("==")) {
            //can only compare stuff which is the same
            ITypeNode otherType = TypeResolver.getTypeTermNode(expr.termNodes.get(0),methodNode,subTable,varTable);
            if(!otherType.getTypeName().equals(type.getTypeName())){
                throw new Exception(getClass().getSimpleName()+": to compare with '==', they have to be the same type ");
            }
            typecheckTermNode(asts,classNode,methodNode,expr.termNodes.get(0),subTable,varTable);
            return;
        }

        if(!currentAllowedTypes.contains(type.getTypeName())){
            if(expr.termNodes.size()==0){
                //we typechecked the type before, as long as it is by itself, there is nothing to do
                return;
            }else{
                throw new Exception(type+" is not in the currently allowed types for use in expression with multiple terms");
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
        if(termNode.termNode instanceof CharConstNode) {
            //nothing to do
        }else if(termNode.termNode instanceof FloatConstNode){
            //nothing to do
        }else if(termNode.termNode instanceof IntConstNode){
            //nothing to do
        }else if(termNode.termNode instanceof ExpressionNode) {

            ExpressionNode expressionNode = (ExpressionNode) termNode.termNode;
            typeCheckExpressionNode(asts,classNode,methodNode,expressionNode,subTable,varTable);

        }else if(termNode.termNode instanceof VariableNode){

            VariableNode variableNode = (VariableNode) termNode.termNode;
            typeCheckVariableNode(asts,classNode,methodNode,variableNode,subTable,varTable);

        }else if(termNode.termNode instanceof MethodCallNode) {

            typeCheckMethodCallNode(asts, classNode, methodNode, (MethodCallNode) termNode.termNode, subTable, varTable);
        }else if(termNode.termNode instanceof ArrayConstantNode){
            typeCheckArrayConstantNode(asts,classNode,methodNode,(ArrayConstantNode) termNode.termNode,subTable,varTable);
        }else{
            throw new Exception("unhandled case "+termNode.termNode.getClass().getName());
        }
    }

    private void typeCheckArrayConstantNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, ArrayConstantNode arrConstNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {
        //all the types of the elements should be the same
        if(arrConstNode.elements.size()>0) {

            ITypeNode type_of_elements = TypeResolver.getTypeExpressionNode(arrConstNode.elements.get(0), methodNode, subTable, varTable);
            for(ExpressionNode expr: arrConstNode.elements){
                ITypeNode element_type = TypeResolver.getTypeExpressionNode(expr,methodNode,subTable,varTable);
                if(!element_type.getTypeName().equals(type_of_elements.getTypeName())){
                    throw new Exception("type of the array items was inferred to "
                            +type_of_elements
                            +" from the first element's type, but type differed at index "
                            +arrConstNode.elements.indexOf(expr)+" : it's type was "+element_type);
                }
                typeCheckExpressionNode(asts,classNode,methodNode,expr,subTable,varTable);
            }
        }
    }

    private void typeCheckVariableNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, VariableNode variableNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
        //it should check that the variable is
        //declared in method scope or class scope.
        //so there should be some declaration of it

        if(variableNode.indexOptional.isPresent()){
            //if there is an index, it should be positive. we can check one of the bounds for free
            //by only accepting PInt type
            ITypeNode index_type = TypeResolver.getTypeExpressionNode(variableNode.indexOptional.get(), methodNode, subTable, varTable);
            if(!index_type.getTypeName().equals("PInt")){
                throw new Exception("can only index into arrays with PInt type. Because an array index is >= 0.");
            }
        }


        //identifiers can only be used within a class or method so
        //there should be a context

        for(ClassFieldNode fieldNode : classNode.fieldNodeList){
            if(fieldNode.name.equals(variableNode.name)){
                //found the identifier declared here
                return;
            }
        }

        //search if identifier is declared as a local variable or argument variable
        if(varTable.containsVariable(variableNode.name)){
            return;
        }

        //search if identifier is declared as a subroutine
        if(subTable.containsSubroutine(variableNode.name)){
            if(!variableNode.indexOptional.isPresent()){
                return;
            }else{
                throw new Exception("TypeChecker: '"+variableNode.name+"' has been used with an index, : '"+variableNode.toSourceCode()+"' , but it is a subroutine. you cannot index into subroutines");
            }
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
        ITypeNode conditionType= TypeResolver.getTypeExpressionNode(whileStatementNode.condition,methodNode,subTable,varTable);
        if(!conditionType.getTypeName().equals("Bool")){
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

        ITypeNode countType= TypeResolver.getTypeExpressionNode(loopStatementNode.count,methodNode,subTable,varTable);
        if( ! countType.getTypeName().equals("PInt") ){
            throw new Exception(" condition should be of an Integral Type >= 0 (PInt) . this is a loop statement after all.");
        }
        for(StatementNode stmt : loopStatementNode.statements){
            typeCheckStatementNode(asts,classNode,methodNode,stmt,subTable,varTable);
        }
    }

    private void typeCheckAssignmentStatementNode(
            List<AST> asts, ClassNode classNode, MethodNode methodNode,
            AssignmentStatementNode assignmentStatementNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable
    ) throws Exception{
        ITypeNode leftSideType = TypeResolver.getTypeVariableNode(assignmentStatementNode.variableNode,methodNode,subTable,varTable);
        ITypeNode rightSideType = TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,varTable);
        if(!leftSideType.getTypeName().equals(rightSideType.getTypeName())){
            throw new Exception(
                    "with an assignment, both sides have to have the same type. here, a value of type "+rightSideType.getTypeName()+" was assigned to a value of type "+leftSideType.getTypeName()
                    +" in source: '"+assignmentStatementNode.toSourceCode()+"'"
            );
        }

        typeCheckVariableNode(asts,classNode,methodNode,assignmentStatementNode.variableNode,subTable,varTable);
        typeCheckExpressionNode(asts,classNode,methodNode,assignmentStatementNode.expressionNode,subTable,varTable);
    }

    public static boolean isIntegralType(ITypeNode type){
        return Arrays.asList("PInt","NInt","Integer").contains(type.getTypeName());
    }

    private void typeCheckTypeIdentifierNode(
            List<AST> asts, ClassNode classNode,
            ITypeNode typename
            ) throws Exception{

        //if it is a simple type, check that the type is defined somewhere
        //so there should exist a class with that type

        if(typename instanceof SimpleTypeNode) {
            if (primitive_types_and_arrays_of_them.contains(typename.getTypeName())) {
                return;
            }

            for (AST ast : asts) {
                for (ClassNode myclassNode : ast.classNodeList) {
                    if (myclassNode.name.getTypeName().equals(typename.getTypeName())) {
                        return;
                    }
                }
            }
        }else if(typename instanceof SubroutineTypeNode) {
            typeCheckTypeIdentifierNode(asts,classNode,((SubroutineTypeNode) typename).returnType);
            for(TypeNode argType : ((SubroutineTypeNode) typename).argumentTypes){
                typeCheckTypeIdentifierNode(asts,classNode,argType.typenode);
            }
            return;
        }

        String msg = TerminalUtil.gererateErrorString("TYPECHECKING: ")
                + "could not find class for type: "
                + StringUtils.wrap(typename.getTypeName(),"'")
                +" in file : (TODO: display file and line number)";
        throw new Exception(msg);
    }

    private void typeCheckMethodNameNode(List<AST> asts, ClassNode classNode, String methodName)throws Exception{
        //method names should not be duplicate in a class
        //this may change in another version of dragon

        long count = classNode.methodNodeList.stream().filter(mNode -> mNode.methodName.equals(methodName)).count();

        if(count>1){
            throw new Exception("duplicate declaration of method '"+methodName+"' ");
        }
    }

    private void typeCheckDeclaredArgumentNode(List<AST> asts, ClassNode classNode, DeclaredArgumentNode declaredArgumentNode)throws Exception{
        typeCheckTypeIdentifierNode(asts,classNode,declaredArgumentNode.type.typenode);
    }

    private void typeCheckIdentifierNode(Set<AST> asts, ClassNode classNode, MethodNode methodNode, IdentifierNode identifierNode)throws Exception{
        //TODO: it should check that the identifier is
        //declared in method scope or class scope.
        //so there should be some declaration of it
        //TODO: also check that the identifier is not duplicate declaration

        //identifiers can only be used within a class or method so
        //there should be a context


        for(ClassFieldNode fieldNode : classNode.fieldNodeList){
            if(fieldNode.name.equals(identifierNode.name)){
                //found the identifier declared here
                return;
            }
        }

        //it is not declared in class scope, it should be declared in
        //method scope

        for(DeclaredArgumentNode arg : methodNode.arguments){
            if(arg.name.equals(identifierNode.name)){
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
