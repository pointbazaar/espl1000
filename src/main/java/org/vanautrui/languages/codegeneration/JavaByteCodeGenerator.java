package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;

import static org.objectweb.asm.Opcodes.*;

public class JavaByteCodeGenerator {
    public static byte[] generateByteCodeForClass(DragonClassNode classNode) {

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
            visitMethodNode(cw,classNode,methodNode);
        }

        cw.visitEnd();

        return cw.toByteArray();
    }

    private static void visitMethodCallNode(ClassWriter cw, MethodVisitor mv, DragonClassNode classNode, DragonMethodNode methodNode, DragonMethodCallNode methodCallNode){
        //TODO: actually compile the stuff, not just fake

        mv.visitFieldInsn(GETSTATIC,
                "java/lang/System",
                "out",
                "Ljava/io/PrintStream;");

        if(methodCallNode.argumentList.size()>0) {
            mv.visitLdcInsn(methodCallNode.argumentList.get(0).str);
        }else{
            mv.visitLdcInsn("");
        }

        switch (methodCallNode.identifierMethodName.name.getContents()) {
            case "println":
                mv.visitMethodInsn(INVOKEVIRTUAL,
                    "java/io/PrintStream",
                    "println",
                    "(Ljava/lang/String;)V");
                break;
            case "print":
                mv.visitMethodInsn(INVOKEVIRTUAL,
                        "java/io/PrintStream",
                        "print",
                        "(Ljava/lang/String;)V");
                break;
        }
    }

    private static void visitStatement(ClassWriter cw,MethodVisitor mv, DragonClassNode classNode, DragonMethodNode methodNode, DragonStatementNode statementNode){
        //TODO: consider other statement types and such
        //statementNode.methodCallNode.visit(mv,classNode,methodNode);
        visitMethodCallNode(cw,mv,classNode,methodNode,statementNode.methodCallNode);
    }

    private static void visitMethodNode(ClassWriter cw, DragonClassNode classNode,DragonMethodNode methodNode){
        String owner = classNode.name.typeName.getContents();
        String descriptor = "i do not know";
        String methodName = methodNode.methodName.methodName.name.getContents();

        //figure it out
        //cw.newMethod(owner,this.methodName.methodName.name.getContents(),descriptor,false);

        //TODO: do not make this static
        //TODO: actually care about it
        //TODO: while typechecking, it should be seen
        //that there is an entry point in the program somewhere
        {
            MethodVisitor mv=cw.visitMethod(ACC_PUBLIC+ACC_STATIC,
                    methodName,
                    "([Ljava/lang/String;)V",
                    null,
                    null);

            //TODO: compile the local variable declarations
            //TODO: compile the statements in the method

            methodNode.statements.forEach(
                    //stmt->stmt.visit(mv,Optional.of(classNode),Optional.of(methodNode))
                    stmt->visitStatement(cw,mv,classNode,methodNode,stmt)
            );

            mv.visitInsn(RETURN);

            //TODO: find out how to compute maxStack
            //and maxLocals

            int maxStack;
            int maxLocals;

            //mv.visitMaxs(maxStack,maxLocals);
            //this apparently tells asm that we want this
            //to be calculated automatically
            //mv.visitMaxs(-1,-1);

            //TODO: this seems to work but it should
            //really be calculated correctly
            mv.visitMaxs(10,10);
            mv.visitEnd();
        }
    }

    private static void visitClassFieldNode(ClassWriter cw, DragonClassNode classNode,DragonClassFieldNode classFieldNode){
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
