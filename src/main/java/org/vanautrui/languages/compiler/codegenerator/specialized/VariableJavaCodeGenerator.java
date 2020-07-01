package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTableRow;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import static org.vanautrui.languages.compiler.codegenerator.specialized.ExpressionJavaCodeGenerator.genDracoVMCodeForExpression;

public final class VariableJavaCodeGenerator {

    static List<String> genJavaCodeForVariable(
            final VariableNode varNode,
            final SymbolTableContext ctx
    ) throws Exception{
        /*generates the code that will compute the variable in an expression.*/

        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;
        final StructsSymbolTable structsTable=ctx.structsTable;

        final List<String> vm = new ArrayList<>();

        if(varTable.containsVariable(varNode.simpleVariableNode.name)) {
            //it is a local variable

            vm.add(varNode.simpleVariableNode.name);

            if (varNode.simpleVariableNode.indexOptional.isPresent()) {
                //it is an array and we should read from the index
                vm.add("["+genDracoVMCodeForExpression(varNode.simpleVariableNode.indexOptional.get(), ctx)+"]");
            }

            //check if there is struct access and do that also
            int i=0;
            while (i<varNode.memberAccessList.size()){
                //access that struct member
                final String memberName=varNode.memberAccessList.get(i).simpleVariableNode.name;

                vm.add("."+memberName);

                if(varNode.memberAccessList.get(i).simpleVariableNode.indexOptional.isPresent()){
                    final ExpressionNode indexIntoMemberExpr = varNode.memberAccessList.get(i).simpleVariableNode.indexOptional.get();
                    vm.add("["+genDracoVMCodeForExpression(indexIntoMemberExpr, ctx)+"]");
                }
                i++;
            }

        }else if(subTable.containsSubroutine(varNode.simpleVariableNode.name) && !varNode.simpleVariableNode.indexOptional.isPresent() && varNode.memberAccessList.size()==0){
            //it is a subroutine
            final String vmcodesubroutinename = SubroutineSymbolTableRow.generateVMCodeSubroutineName(subTable.getContainingClassName(varNode.simpleVariableNode.name),varNode.simpleVariableNode.name);

            //TODO: figure out how to write a reference to a subroutine as an expression
            throw new RuntimeException("NOT IMPLEMENTED");
        }else {
            throw new Exception(
                    "DracoVMCodeGenerator: '"
                    +varNode.simpleVariableNode.name
                    +"' was not found in local variable symbol table  and also not found in subroutine symbol table. or some other stuff happened"
            );
        }
        return vm;
    }
}
