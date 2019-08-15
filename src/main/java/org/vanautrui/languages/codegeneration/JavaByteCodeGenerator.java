package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassFieldNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;

import static org.objectweb.asm.Opcodes.*;

public class JavaByteCodeGenerator {

    //https://github.com/Konloch/bytecode-viewer/releases/tag/v2.9.22
    //using this bytecode viewer to see the results

    //https://www.javaworld.com/article/2077193/the-java-class-file-lifestyle.html

    //https://docs.oracle.com/javase/specs/jvms/se8/html/index.html

    //https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-2.html#jvms-2.1

    public static byte[] generateByteCodeForClass(DragonClassNode classNode) throws Exception {

        ClassWriter cw = new ClassWriter(0);

        //cw.newClass(this.name.typeName.getContents());

        //TODO: handle access modifiers
        //TODO: handle other modifiers

        {
            int access = ACC_SUPER;
            if (classNode.access.is_public) {
                access += ACC_PUBLIC;
            } else {
                access += ACC_PRIVATE;
            }

            int classFileVersion = 49;

            String superClassName = "java/lang/Object";

            cw.visit(classFileVersion,
                    access,
                    classNode.name.typeName.getContents(),
                    null,
                    superClassName,
                    null);
        }

        MethodVisitor mv;

        {
            //write the default constructor
            mv=cw.visitMethod(ACC_PUBLIC,"<init>","()V",null,null);
            mv.visitVarInsn(ALOAD,0);
            mv.visitMethodInsn(INVOKESPECIAL,
                    "java/lang/Object",
                    "<init>",
                    "()V");
            mv.visitInsn(RETURN);
            mv.visitMaxs(1,1);
            mv.visitEnd();
        }

        for(DragonClassFieldNode fieldNode : classNode.fieldNodeList){

            //fieldNode.visit(cw, Optional.of(classNode),Optional.empty());
            visitClassFieldNode(cw,classNode,fieldNode);
        }

        for(DragonMethodNode methodNode : classNode.methodNodeList){
            //methodNode.visit(cw,Optional.of(classNode),Optional.empty());
            DragonMethodCodeGenerator.visitMethodNode(cw,classNode,methodNode);
        }

        cw.visitEnd();

        return cw.toByteArray();
    }



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
        if(loop.count.term.integerConstantNode.isPresent() ) {
            DragonIntegerConstantNode integerConstantNode = (DragonIntegerConstantNode)loop.count.term.integerConstantNode.get();
            mv.visitIntInsn(BIPUSH, (int)integerConstantNode.value);
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
        mv.visitIntInsn(BIPUSH,1);
        mv.visitInsn(ISUB);

        mv.visitJumpInsn(GOTO,start);
        mv.visitLabel(end);

        //remove the loop counter
        mv.visitInsn(POP);
    }





    public static void visitClassFieldNode(ClassWriter cw, DragonClassNode classNode,DragonClassFieldNode classFieldNode){
        String owner = classNode.name.typeName.getContents();
        //TODO: figure out if we are doing this correctly, i doubt it
        String descriptor="i dont know";
        cw.newField(owner,classFieldNode.name.name.getContents(),descriptor);
    }

    //it would be better not to have all the code generation
    //logic in the AST,
    //as it would crowd the AST classes.
    //seperate classes for code generation would
    //be more appropriate in my opinion
    //also, because there may be multiple
    //target languages,
    //and because code generation may require
    //looking at many things
    //in the AST at once, which is not easily
    //supported by its tree structure


}
