package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassFieldNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import static org.vanautrui.languages.symboltablegenerator.SymbolTableGenerator.*;
import static org.objectweb.asm.Opcodes.*;

public class JavaByteCodeGenerator {

    //https://github.com/Konloch/bytecode-viewer/releases/tag/v2.9.22
    //using this bytecode viewer to see the results

    //https://www.javaworld.com/article/2077193/the-java-class-file-lifestyle.html

    //https://docs.oracle.com/javase/specs/jvms/se8/html/index.html

    //https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-2.html#jvms-2.1

    public static byte[] generateByteCodeForOneClass(ClassNode classNode, boolean debug)throws Exception{
        SubroutineSymbolTable subroutineSymbolTable = createSubroutineSymbolTable(classNode);

        return generateByteCodeForClass(classNode,subroutineSymbolTable,debug);
    }

    public static byte[] generateByteCodeForClass(ClassNode classNode, SubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception {

        if(debug){
            System.out.println(subroutineSymbolTable.toString());
        }

        ClassWriter cw = new ClassWriter(0);

        //cw.newClass(this.name.typeName.getContents());

        //TODO: handle access modifiers
        //TODO: handle other modifiers

        {
            int access = ACC_SUPER;
            if (classNode.isPublic) {
                access += ACC_PUBLIC;
            } else {
                access += ACC_PRIVATE;
            }

            int classFileVersion = 49;

            String superClassName = "java/lang/Object";

            cw.visit(classFileVersion,
                    access,
                    classNode.name.typeName,
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

        for(ClassFieldNode fieldNode : classNode.fieldNodeList){

            //fieldNode.visit(cw, Optional.of(classNode),Optional.empty());
            visitClassFieldNode(cw,classNode,fieldNode);
        }

        //first create the subroutine symbol table,
        //so each method knows which methods it can call
        //TODO: do this previous to compilation for all
        //classes since a method can call methods from another class



        for(MethodNode methodNode : classNode.methodNodeList){
            //methodNode.visit(cw,Optional.of(classNode),Optional.empty());
            MethodCodeGenerator.visitMethodNode(cw,classNode,methodNode,subroutineSymbolTable,debug);
        }

        cw.visitEnd();

        return cw.toByteArray();
    }

    
	public static void pushFloatConstant(float f, MethodVisitor mv){
		mv.visitLdcInsn(f);
	}









    public static void visitClassFieldNode(ClassWriter cw, ClassNode classNode, ClassFieldNode classFieldNode){
        String owner = classNode.name.typeName;
        //TODO: figure out if we are doing this correctly, i doubt it
        String descriptor="i dont know";
        cw.newField(owner,classFieldNode.name.name,descriptor);
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
