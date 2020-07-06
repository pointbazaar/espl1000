package org.vanautrui.languages.compiler.codegenerator;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.*;

import static org.vanautrui.languages.compiler.codegenerator.specialized.ExpressionJavaCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.codegenerator.specialized.SubroutineJavaCodeGenerator.generateJavaCodeForMethod;

public final class JavaCodeGenerator {

    //prevents instance creation
    private JavaCodeGenerator(){}

    public static Map<String, List<String>> generateJavaCode(
            final AST_Whole_Program asts,
            final SubroutineSymbolTable subTable,
            final StructsSymbolTable structsTable,
            final boolean debug,
            final boolean printsymboltables
    ) throws Exception {

        final Map<String,List<String>> instructions = new HashMap<>();

        for(final NamespaceNode namespaceNode : asts.namespaceNodeList){

            final List<String> classLines = new ArrayList<>();

            for(final MethodNode methodNode : namespaceNode.methods){

                //namespaceNode, methodNode, writer are not accessed from other threads
                //debug, printsymboltables are only read, not written to.
                //subTable, structsTable are probably only read from, but need to be synchronized,
                //as they are important to all threads.
                final List<String> javaLinesForMethod = generateJavaCodeForMethod(namespaceNode, methodNode, subTable, structsTable, debug, printsymboltables);
                classLines.addAll(javaLinesForMethod);
            }
            instructions.put(namespaceNode.name,classLines);
        }
        return instructions;
    }

    public static List<String> genJavaCodeForFloatConst(final float fconst){
        return Arrays.asList(""+fconst);
    }

    public static List<String> genJavaCodeForIntConst(final int iconst){
        return Arrays.asList(""+iconst);
    }

    public static List<String> genJavaCodeForBoolConst(final BoolConstNode bconst){
        return Arrays.asList(""+bconst);
    }

    /**
     * after this subroutine, the address of the array with the specified elements inside is on the stack
     * @param arrayConstantNode
     * @throws Exception
     */
    public static List<String> genJavaCodeForArrayConstant(
            final ArrayConstantNode arrayConstantNode,
            final SymbolTableContext ctx
    ) throws Exception{
        //TODO: figure out the type, then make appropriate java array constant
        final List<String> vm = new ArrayList<>();

        for(int i=0;i<arrayConstantNode.elements.size();i++) {

            vm.add(genDracoVMCodeForExpression(arrayConstantNode.elements.get(i), ctx));
        }
        throw new RuntimeException("NOT IMPLEMENTED");
    }

    public static long unique(){
        //uniqueness for jump labels
        return Math.abs((new Random()).nextInt(100000));
    }
}
