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
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.List;
import java.util.Optional;
import java.util.Random;
import java.util.Set;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.SubroutineDracoVMCodeGenerator.generateDracoVMCodeForMethod;

public final class DracoVMCodeGenerator {

    public static List<String> generateDracoVMCode(
            Set<AST> asts,
            SubroutineSymbolTable subTable,
            StructsSymbolTable structsTable,
            boolean debug,boolean printsymboltables
    ) throws Exception{

        DracoVMCodeWriter sb = new DracoVMCodeWriter();
        for(AST ast :asts){
            for(NamespaceNode namespaceNode : ast.namespaceNodeList){
                for(MethodNode methodNode : namespaceNode.methodNodeList){
                    generateDracoVMCodeForMethod(namespaceNode,methodNode,sb,subTable, structsTable,debug,printsymboltables);
                }
            }
        }
        return sb.getDracoVMCodeInstructions();
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
    public static void genVMCodeForArrayConstant(
            ArrayConstantNode arrayConstantNode,
            DracoVMCodeWriter sb,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception{

        //allocate space for the new array.
        //this leaves the address of the new array (the new array resides on the heap) on the stack

        sb.iconst(arrayConstantNode.elements.size()); //amount of DWORD's to reserve
        sb.call("Builtin","new"); //should leave the address to the newly allocated space on the stack

        //caller removes the arguments
        sb.swap("remove previously pushed arguments");
        sb.pop("remove previously pushed arguments");

        //put the individual elements into the array
        for(int i=0;i<arrayConstantNode.elements.size();i++) {
            sb.dup(); //duplicate the array address as it is consumed with  'arraystore'

            sb.iconst(i);//index to store into

            //value we want to store
            genDracoVMCodeForExpression(arrayConstantNode.elements.get(i),sb,subTable,varTable,structsTable);

            sb.arraystore();
        }
    }

    public static long unique(){
        //uniqueness for jump labels
        Random r = new Random();
        return Math.abs(r.nextInt(100000));
    }
}
