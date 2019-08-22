package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;

import static org.objectweb.asm.Opcodes.*;

public class WhileStatementCodeGenerator {

    public static void visitWhileStatmentNode(
            ClassWriter cw,
            MethodVisitor mv,
            ClassNode classNode,
            MethodNode methodNode,
            WhileStatementNode whileStatementNode,
            DragonMethodScopeVariableSymbolTable methodScopeSymbolTable,
            DragonSubroutineSymbolTable subroutineSymbolTable,
            boolean debug
    ) throws Exception {
        //https://asm.ow2.io/asm4-guide.pdf
        //https://en.wikipedia.org/wiki/Java_bytecode_instruction_listings

        //idea:
        /*

        labelStart:
            evaluate condition
            if condition is false (0 on the stack) then goto labelEnd

            execute statements in body
            goto LabelStart
        labelEnd:

         */

        Label startLabel=new Label();
        Label endLabel=new Label();

        mv.visitLabel(startLabel);

        ExpressionCodeGenerator.visitExpression(cw,mv,
                classNode,methodNode,
                whileStatementNode.condition,methodScopeSymbolTable,
                subroutineSymbolTable,debug);

        mv.visitJumpInsn(IFEQ,endLabel);

        for(StatementNode stmt : whileStatementNode.statements) {
            StatementCodeGenerator.visitStatement(cw,mv,classNode,methodNode,stmt, subroutineSymbolTable, methodScopeSymbolTable,debug);
        }

        mv.visitJumpInsn(GOTO,startLabel);


        //because the JVM gives a VerifyError if there is a label but no instructions after it.
        //thats why there is a NoOPeration instruction here
        mv.visitLabel(endLabel);
        mv.visitInsn(NOP);
    }
}
