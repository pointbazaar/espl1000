package org.vanautrui.languages.compiler.vmcodegenerator;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.FloatConstNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTableRow;

import java.util.List;
import java.util.Optional;
import java.util.Random;
import java.util.Set;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.SubroutineDracoVMCodeGenerator.generateDracoVMCodeForMethod;

public class DracoVMCodeGenerator {

    public static List<String> generateDracoVMCode(Set<AST> asts, SubroutineSymbolTable subTable,boolean debug,boolean printsymboltables) throws Exception{

        DracoVMCodeWriter sb = new DracoVMCodeWriter();
        for(AST ast :asts){
            for(NamespaceNode namespaceNode : ast.namespaceNodeList){
                for(MethodNode methodNode : namespaceNode.methodNodeList){
                    generateDracoVMCodeForMethod(namespaceNode,methodNode,sb,subTable,debug,printsymboltables);
                }
            }
        }
        return sb.getDracoVMCodeInstructions();
    }

    /**
     * @param assignStmt the AssignmentStatementNode being compiled
     * @param sb         the VM Code Writer class
     * @param varTable   the Local Variable Symbol Table
     * @throws Exception if the variable is not in the symbol table
     */
    public static void genVMCodeForAssignmentStatement(AssignmentStatementNode assignStmt, DracoVMCodeWriter sb,SubroutineSymbolTable subTable,LocalVarSymbolTable varTable) throws Exception {

        //the variable being assigned to would be a local variable or argument.
        //the expression that is being assigned, there can be code generated to put it on the stack
        final String varName = assignStmt.variableNode.name;
        final String segment = varTable.getSegment(varName);
        final int index = varTable.getIndexOfVariable(varName);

        if(assignStmt.variableNode.indexOptional.isPresent()){
            //push the array address on the stack
            sb.push(segment,index);

            //push the index
            genDracoVMCodeForExpression(assignStmt.variableNode.indexOptional.get(),sb,subTable,varTable);

            //push the value we want to store
            genDracoVMCodeForExpression(assignStmt.expressionNode,sb,subTable,varTable);

            sb.arraystore();
        }else {
            genDracoVMCodeForExpression(assignStmt.expressionNode,sb,subTable,varTable);
            //then we just pop that value into the appropriate segment with the specified index
            sb.pop(segment, index);
        }
    }


    public static void genVMCodeForFloatConst(FloatConstNode fconst,DracoVMCodeWriter sb){
        sb.fconst(fconst.value);
    }

    public static void genVMCodeForIntConst(int iconst,DracoVMCodeWriter sb){
        sb.iconst(iconst);
    }

    public static void genVMCodeForBoolConst(BoolConstNode bconst,DracoVMCodeWriter sb){
        sb.iconst((bconst.value)?1:0);
    }


    /**
     * after this subroutine, the address of the array with the specified elements inside is on the stack
     * @param arrayConstantNode
     * @param sb
     * @param subTable
     * @param varTable
     * @throws Exception
     */
    public static void genVMCodeForArrayConstant(ArrayConstantNode arrayConstantNode, DracoVMCodeWriter sb, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{

        //allocate space for the new array.
        //this leaves the address of the new array (the new array resides on the heap) on the stack

        sb.iconst(arrayConstantNode.elements.size()); //amount of DWORD's to reserve
        sb.call("Builtin","malloc"); //should leave the address to the newly allocated space on the stack

        //caller removes the arguments
        sb.swap("remove previously pushed arguments");
        sb.pop("remove previously pushed arguments");

        //put the individual elements into the array
        for(int i=0;i<arrayConstantNode.elements.size();i++) {
            sb.dup(); //duplicate the array address as it is consumed with  'arraystore'

            sb.iconst(i);//index to store into

            //value we want to store
            genDracoVMCodeForExpression(arrayConstantNode.elements.get(i),sb,subTable,varTable);

            sb.arraystore();
        }
    }

    public static void genDracoVMCodeForVariable(String varName, Optional<ExpressionNode> indexOptional, DracoVMCodeWriter sb, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
        //push the variable on the stack

        //final String name = variableNode.name;

        if(varTable.containsVariable(varName)) {
            //if the variable is local,
            //or argument, that would be different segments
            //we have to consider its index

            //it is a local variable
            final int index = varTable.getIndexOfVariable(varName);
            final String segment = varTable.getSegment(varName);

            sb.push(segment, index);

            if (indexOptional.isPresent()) {
                //it is an array and we should read from the index
                genDracoVMCodeForExpression(indexOptional.get(), sb, subTable, varTable);
                sb.arrayread();
            }
        }else if(subTable.containsSubroutine(varName)){
            //it is a subroutine
            String vmcodesubroutinename = SubroutineSymbolTableRow.generateVMCodeSubroutineName(subTable.getContainingClassName(varName),varName);
            //push the corresponding label on the stack
            sb.pushsubroutine(vmcodesubroutinename);
        }else {
            throw new Exception("DracoVMCodeGenerator: '"+varName+"' was not found in local variable symbol table  and also not found in subroutine symbol table.");
        }
    }






    public static long unique(){
        //uniqueness for jump labels
        Random r = new Random();
        return Math.abs(r.nextInt(100000));
    }
}
