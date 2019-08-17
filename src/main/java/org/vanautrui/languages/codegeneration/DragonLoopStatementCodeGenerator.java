package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.JavaByteCodeGenerator.pushIntegerConstant;

public class DragonLoopStatementCodeGenerator {

    public static void visitLoopStatmentNode(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonLoopStatementNode loop,
            DragonMethodScopeSymbolTable methodScopeSymbolTable
    ) throws Exception {
        //https://asm.ow2.io/asm4-guide.pdf
        //https://en.wikipedia.org/wiki/Java_bytecode_instruction_listings

        //TODO: actually compile the stuff, not just fake

        //loop.count;

        //loop.statements
        Label start = new Label();
        Label end=new Label();

        //push our loop counter
        if(loop.count.term.termNode instanceof DragonIntegerConstantNode) {
            DragonIntegerConstantNode integerConstantNode = (DragonIntegerConstantNode)loop.count.term.termNode;

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
        for(DragonStatementNode stmt : loop.statements){
            DragonStatementCodeGenerator.visitStatement(cw,mv,classNode,methodNode,stmt,methodScopeSymbolTable);
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
