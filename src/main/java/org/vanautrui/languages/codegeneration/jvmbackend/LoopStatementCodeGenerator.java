package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.IntegerConstantNode;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.jvmbackend.IntegerConstantCodeGenerator.pushIntegerConstant;

public class LoopStatementCodeGenerator {

    public static void visitLoopStatmentNode(
            ClassWriter cw,
            MethodVisitor mv,
            ClassNode classNode,
            MethodNode methodNode,
            LoopStatementNode loop,
            LocalVarSymbolTable methodScopeSymbolTable,
            SubroutineSymbolTable subroutineSymbolTable,
            boolean debug
    ) throws Exception {
        //https://asm.ow2.io/asm4-guide.pdf
        //https://en.wikipedia.org/wiki/Java_bytecode_instruction_listings

        //TODO: actually compile the stuff, not just fake

        //loop.count;

        //loop.statements
        Label start = new Label();
        Label end=new Label();

        //push our loop counter
        if(loop.count.term.termNode instanceof IntegerConstantNode) {
            IntegerConstantNode integerConstantNode = (IntegerConstantNode)loop.count.term.termNode;

            //mv.visitIntInsn(BIPUSH, integerConstantNode.value);
            pushIntegerConstant(integerConstantNode.value,mv);
        }else{
            //TODO: deal with the other cases
            throw new Exception(" not implemented yet");
        }

        mv.visitLabel(start);

        //duplicate the condition (top of stack)
        //so that the if doesnt remove it for the next iteration
        mv.visitInsn(DUP);

        //if count <= 0 , goto end
        mv.visitJumpInsn(IFLE,end);

        //write the code for the statements
        for(StatementNode stmt : loop.statements){
            StatementCodeGenerator.visitStatement(cw,mv,classNode,methodNode,stmt, subroutineSymbolTable,methodScopeSymbolTable,debug);
        }

        //decrement the loop counter : count--;

        //mv.visitIntInsn(BIPUSH,1);
        pushIntegerConstant(1,mv);

        mv.visitInsn(ISUB);

        mv.visitJumpInsn(GOTO,start);
        mv.visitLabel(end);

        //remove the loop counter
        mv.visitInsn(POP);
    }
}
