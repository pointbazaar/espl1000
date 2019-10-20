package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;

public final class AssignmentStatementDracoVMCodeGenerator {

    /**
     * @param assignStmt the AssignmentStatementNode being compiled
     * @param sb         the VM Code Writer class
     * @param varTable   the Local Variable Symbol Table
     * @throws Exception if the variable is not in the symbol table
     */
    public static void genVMCodeForAssignmentStatement(
            AssignmentStatementNode assignStmt,
            DracoVMCodeWriter sb,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable
    ) throws Exception {

        if(assignStmt.variableNode.memberAccessList.size()==0){
            genVMCodeForSimpleAssignmentStatementWithoutStructAccess(assignStmt.variableNode.simpleVariableNode,assignStmt.expressionNode,sb,subTable,varTable);
        }else{
            genVMCodeForAssignmentStatementInGeneral(assignStmt,sb,subTable,varTable);
        }
    }

    private static void genVMCodeForAssignmentStatementInGeneral(
            AssignmentStatementNode assignStmt,
            DracoVMCodeWriter sb,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable
    ) throws Exception {
        //TODO

        todo
    }

    private static void genVMCodeForSimpleAssignmentStatementWithoutStructAccess(
            SimpleVariableNode varNode,
            ExpressionNode expr,
            DracoVMCodeWriter sb,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable
    )throws Exception{
        //the variable being assigned to would be a local variable or argument.
        //the expression that is being assigned, there can be code generated to put it on the stack
        final String varName = varNode.name;
        final String segment = varTable.getSegment(varName);
        final int index = varTable.getIndexOfVariable(varName);

        if(varNode.indexOptional.isPresent()){
            //push the array address on the stack
            sb.push(segment,index);

            //push the index
            genDracoVMCodeForExpression(varNode.indexOptional.get(),sb,subTable,varTable);

            //push the value we want to store
            genDracoVMCodeForExpression(expr,sb,subTable,varTable);

            sb.arraystore();
        }else {
            genDracoVMCodeForExpression(expr,sb,subTable,varTable);
            //then we just pop that value into the appropriate segment with the specified index
            sb.pop(segment, index);
        }
    }
}
