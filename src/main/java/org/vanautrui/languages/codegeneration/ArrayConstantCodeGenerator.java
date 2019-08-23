package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.BoolConstantNode;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.typeresolution.TypeResolver;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.IntegerConstantCodeGenerator.pushIntegerConstant;

public class ArrayConstantCodeGenerator {

    public static void visitArrayConstant(
            ClassWriter cw,
            MethodVisitor mv,
            ClassNode classNode,
            MethodNode methodNode,
            ArrayConstantNode arr,
            LocalVarSymbolTable varTable,
            SubroutineSymbolTable subTable,
            boolean debug
    ) throws Exception {
        //TODO

        //i decompiled a java class file where i just created an array with 4 integers.
        //studying the bytecode makes it easy to find implementation for this language

        //push array size
        //newarray $number_i_dont_understand_to_identify_type_of_elements //this number is 10 for Int
        //
        //for each element in array.elements
        //  dup
        //  push index of that element in the array
        //  code for pushing element on the stack
        //  iastore //for int
        //
        //

        //for later, for the assignment statement
        // //astore //to
        mv.visitLabel(new Label());
        
        int size=arr.elements.size();
        pushIntegerConstant(size,mv);



        mv.visitIntInsn(NEWARRAY,10);
        //TODO: figure out what the operand means (means array of int) and figure out
        //for other types



        for(int i=0;i<arr.elements.size();i++){
            mv.visitInsn(DUP);
            pushIntegerConstant(i,mv);
            ExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,arr.elements.get(i),varTable,subTable,debug);

            String type = TypeResolver.getTypeExpressionNode(arr.elements.get(i), methodNode, subTable, varTable);
            switch (type){
                case "Int":
                    mv.visitInsn(IASTORE);
                    break;
                case "Float":
                    mv.visitInsn(FASTORE);
                    break;
                default:
                    throw new Exception("unhandled case in ArrayConstantCodeGenerator.java");
            }
        }
    }
}
