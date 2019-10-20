package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTableRow;
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
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception {

        if(assignStmt.variableNode.memberAccessList.size()==0){
            genVMCodeForSimpleAssignmentStatementWithoutStructAccess(assignStmt.variableNode.simpleVariableNode,assignStmt.expressionNode,sb,subTable,varTable,structsTable);
        }else{
            genVMCodeForAssignmentStatementWithStructAccess(assignStmt,sb,subTable,varTable,structsTable);
        }
    }

    private static void genVMCodeForAssignmentStatementWithStructAccess(
            AssignmentStatementNode assignStmt,
            DracoVMCodeWriter sb,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception {
        //TODO: dereference up until the last member access / last index access,
        //then use arraystore to store what is being assigned

        /*
        for example
        x.b=3
            put x on stack
            use arraystore to store into it's member b

        x.b[1]=2
            put x on stack
            arrayread to dereference b
            arraystore to store into it's index 1
         */

        //so this is just like putting the variable onto the stack,
        //except at the last part, there is something being stored

        final VariableNode varNode = assignStmt.variableNode;
        final ExpressionNode expr = assignStmt.expressionNode;

        //stores the type of the previous value so we know
        String previous_type=varTable.getTypeOfVariable(varNode.simpleVariableNode.name).getTypeName();

        //it is a local variable
        final int index = varTable.getIndexOfVariable(varNode.simpleVariableNode.name);
        final String segment = varTable.getSegment(varNode.simpleVariableNode.name);

        sb.push(segment, index);

        if (varNode.simpleVariableNode.indexOptional.isPresent()) {
            //it is an array and we should read from the index
            genDracoVMCodeForExpression(varNode.simpleVariableNode.indexOptional.get(), sb, subTable, varTable,structsTable);
            sb.arrayread();
        }

        //check if there is struct access and do that also
        int i=0;
        while (i<varNode.memberAccessList.size()-1){
            //access that struct member, and its index in the struct
            final String memberName=varNode.memberAccessList.get(i).name;

            //figure out which struct
            final StructsSymbolTableRow struct = structsTable.get(previous_type);
            final int indexOfMember = struct.getIndexOfMember(memberName);

            sb.iconst(indexOfMember);
            sb.arrayread();

            //for the next one
            previous_type = struct.getTypeOfMember(memberName);

            if(varNode.memberAccessList.get(i).indexOptional.isPresent()){
                final ExpressionNode indexIntoMemberExpr = varNode.memberAccessList.get(i).indexOptional.get();
                ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression(indexIntoMemberExpr,sb,subTable,varTable,structsTable);
                sb.arrayread();

                //unwrap our previous type, as we have indexed into it
                previous_type=previous_type.substring(1,previous_type.length()-1);
            }

            i++;
        }

        //now we have .x or .x[4] left. if we have an index on the last one, we want to dereference one more time.
        final SimpleVariableNode last = varNode.memberAccessList.get(varNode.memberAccessList.size()-1);
        if(last.indexOptional.isPresent()){
            //access that struct member, and its index in the struct
            final String memberName=last.name;

            //figure out which struct
            final StructsSymbolTableRow struct = structsTable.get(previous_type);
            final int indexOfMember = struct.getIndexOfMember(memberName);

            sb.iconst(indexOfMember);
            sb.arrayread();

            //for the next one
            previous_type = varTable.getTypeOfVariable(last.name).getTypeName();
        }

        //push the index to store into, wich is
        //  an index into a struct or
        //  an index into an array

        if(last.indexOptional.isPresent()){
            //push the index into the array
            genDracoVMCodeForExpression(last.indexOptional.get(),sb,subTable,varTable,structsTable);
        }{
            //push the index into the struct

            final String memberName=last.name;
            final StructsSymbolTableRow struct = structsTable.get(previous_type);
            final int indexOfMember = struct.getIndexOfMember(memberName);

            sb.iconst(indexOfMember);
        }

        genDracoVMCodeForExpression(expr,sb,subTable,varTable,structsTable);
        sb.arraystore();
    }

    private static void genVMCodeForSimpleAssignmentStatementWithoutStructAccess(
            SimpleVariableNode varNode,
            ExpressionNode expr,
            DracoVMCodeWriter sb,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
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
            genDracoVMCodeForExpression(varNode.indexOptional.get(),sb,subTable,varTable,structsTable);

            //push the value we want to store
            genDracoVMCodeForExpression(expr,sb,subTable,varTable,structsTable);

            sb.arraystore();
        }else {
            genDracoVMCodeForExpression(expr,sb,subTable,varTable,structsTable);
            //then we just pop that value into the appropriate segment with the specified index
            sb.pop(segment, index);
        }
    }
}
