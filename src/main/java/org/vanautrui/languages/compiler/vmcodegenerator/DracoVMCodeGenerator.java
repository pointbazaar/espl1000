package org.vanautrui.languages.compiler.vmcodegenerator;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.*;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.SubroutineDracoVMCodeGenerator.generateDracoVMCodeForMethod;

public final class DracoVMCodeGenerator {

    //prevents instance creation
    private DracoVMCodeGenerator(){}

    public static Map<String, List<String>> generateDracoVMCode(
            final AST_Whole_Program asts,
            final SubroutineSymbolTable subTable,
            final StructsSymbolTable structsTable,
            final boolean debug,
            final boolean printsymboltables
    ) throws Exception {

        final Map<String,List<String>> dracovmcodeinstructions = new HashMap<>();

        for(final NamespaceNode namespaceNode : asts.namespaceNodeList){
            for(final MethodNode methodNode : namespaceNode.methodNodeList){

                //namespaceNode, methodNode, writer are not accessed from other threads
                //debug, printsymboltables are only read, not written to.
                //subTable, structsTable are probably only read from, but need to be synchronized,
                //as they are important to all threads.
                final List<String> subr_vm_codes = generateDracoVMCodeForMethod(namespaceNode, methodNode, subTable, structsTable, debug, printsymboltables);
                dracovmcodeinstructions.put(namespaceNode.name+"_"+methodNode.methodName,subr_vm_codes);
            }

        }

        return dracovmcodeinstructions;
    }

    public static List<String> genVMCodeForFloatConst(final float fconst){
        return Arrays.asList("fconst "+fconst);
    }

    public static List<String> genVMCodeForIntConst(final int iconst){
        return Arrays.asList("iconst "+iconst);
    }

    public static List<String> genVMCodeForBoolConst(final BoolConstNode bconst){
        return Arrays.asList("iconst "+((bconst.value)?1:0));
    }

    /**
     * after this subroutine, the address of the array with the specified elements inside is on the stack
     * @param arrayConstantNode
     * @throws Exception
     */
    public static List<String> genVMCodeForArrayConstant(
            final ArrayConstantNode arrayConstantNode,
            final SymbolTableContext ctx
    ) throws Exception{

        final List<String> vm = new ArrayList<>();

        //allocate space for the new array.
        //this leaves the address of the new array (the new array resides on the heap) on the stack
        vm.add("iconst "+arrayConstantNode.elements.size());//amount of DWORD's to reserve

        vm.add("call Builtin_new"); //should leave the address to the newly allocated space on the stack

        //caller removes the arguments
        vm.add("swap"); //remove previously pushed arguments");
        vm.add("pop");  //remove previously pushed arguments");

        //put the individual elements into the array
        for(int i=0;i<arrayConstantNode.elements.size();i++) {
            //duplicate the array address as it is consumed with  'arraystore'
            vm.add("dup");

            vm.add("iconst "+i);//index to store into

            //value we want to store
            vm.addAll(genDracoVMCodeForExpression(arrayConstantNode.elements.get(i), ctx));

            vm.add("arraystore");
        }
        return vm;
    }

    public static long unique(){
        //uniqueness for jump labels
        return Math.abs((new Random()).nextInt(100000));
    }
}
