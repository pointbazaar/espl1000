package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

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

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;

public final class VariableDracoVMCodeGenerator {

    static List<String> genDracoVMCodeForSimpleVariable(
            final String varName,
            final Optional<ExpressionNode> indexOptional,
            final SymbolTableContext ctx
    ) throws Exception{

        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;

        final List<String> vm = new ArrayList<>();

        //push the variable on the stack

        if(varTable.containsVariable(varName)) {
            //if the variable is local,
            //or argument, that would be different segments
            //we have to consider its index

            //it is a local variable
            final int index = varTable.getIndexOfVariable(varName);
            final String segment = varTable.getSegment(varName);

            vm.add("push "+segment+" "+index);

            if (indexOptional.isPresent()) {
                //it is an array and we should read from the index
                vm.addAll(genDracoVMCodeForExpression(indexOptional.get(), ctx));
                vm.add("arrayread");
            }
        }else if(subTable.containsSubroutine(varName)){
            //it is a subroutine
            final String vmcodesubroutinename = SubroutineSymbolTableRow.generateVMCodeSubroutineName(subTable.getContainingClassName(varName),varName);
            //push the corresponding label on the stack
            vm.add("pushsubroutine "+vmcodesubroutinename);
        }else {
            throw new Exception("DracoVMCodeGenerator: '"+varName+"' was not found in local variable symbol table  and also not found in subroutine symbol table.");
        }

        return vm;
    }

    static List<String> genDracoVMCodeForVariable(
            final VariableNode varNode,
            final SymbolTableContext ctx
    ) throws Exception{


        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;
        final StructsSymbolTable structsTable=ctx.structsTable;

        final List<String> vm = new ArrayList<>();
        //push the variable on the stack

        //final String name = variableNode.name;

        if(varTable.containsVariable(varNode.simpleVariableNode.name)) {
            //if the variable is local,
            //or argument, that would be different segments
            //we have to consider its index

            //it is a local variable
            final int index = varTable.getIndexOfVariable(varNode.simpleVariableNode.name);
            final String segment = varTable.getSegment(varNode.simpleVariableNode.name);

            vm.add("push "+segment+" "+index);

            if (varNode.simpleVariableNode.indexOptional.isPresent()) {
                //it is an array and we should read from the index
                vm.addAll(genDracoVMCodeForExpression(varNode.simpleVariableNode.indexOptional.get(), ctx));
                vm.add("arrayread");
            }

            //check if there is struct access and do that also
            int i=0;
            while (i<varNode.memberAccessList.size()){
                //access that struct member, and maybe its index also
                final String memberName=varNode.memberAccessList.get(i).name;

                //figure out which struct
                final TypeNode firstType = varTable.getTypeOfVariable(varNode.simpleVariableNode.name);
                final StructsSymbolTableRow struct = structsTable.get(firstType.getTypeName());
                final int indexOfMember = struct.getIndexOfMember(memberName);

                vm.add("iconst "+indexOfMember);
                vm.add("arrayread");

                if(varNode.memberAccessList.get(i).indexOptional.isPresent()){
                    final ExpressionNode indexIntoMemberExpr = varNode.memberAccessList.get(i).indexOptional.get();
                    vm.addAll(genDracoVMCodeForExpression(indexIntoMemberExpr, ctx));
                    vm.add("arrayread");
                }
                i++;
            }

        }else if(subTable.containsSubroutine(varNode.simpleVariableNode.name) && !varNode.simpleVariableNode.indexOptional.isPresent() && varNode.memberAccessList.size()==0){
            //it is a subroutine
            final String vmcodesubroutinename = SubroutineSymbolTableRow.generateVMCodeSubroutineName(subTable.getContainingClassName(varNode.simpleVariableNode.name),varNode.simpleVariableNode.name);
            //push the corresponding label on the stack
            vm.add("pushsubroutine "+vmcodesubroutinename);
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
