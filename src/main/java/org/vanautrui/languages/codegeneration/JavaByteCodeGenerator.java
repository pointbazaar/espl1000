package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.rows.DragonSubroutineSymbolTableRow;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassFieldNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

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

        //first create the subroutine symbol table,
        //so each method knows which methods it can call
        //TODO: do this previous to compilation for all
        //classes since a method can call methods from another class

        DragonSubroutineSymbolTable subroutineSymbolTable = createSubroutineSymbolTable(classNode);

        for(DragonMethodNode methodNode : classNode.methodNodeList){
            //methodNode.visit(cw,Optional.of(classNode),Optional.empty());
            DragonMethodCodeGenerator.visitMethodNode(cw,classNode,methodNode,subroutineSymbolTable);
        }

        cw.visitEnd();

        return cw.toByteArray();
    }

    private static DragonSubroutineSymbolTable createSubroutineSymbolTable(DragonClassNode classNode){
        DragonSubroutineSymbolTable subroutineSymbolTable = new DragonSubroutineSymbolTable();

        for(DragonMethodNode methodNode : classNode.methodNodeList){
            DragonSubroutineSymbolTableRow subrRow = new DragonSubroutineSymbolTableRow(methodNode.methodName.methodName.name.getContents(),methodNode.type.typeName.getContents());
            subroutineSymbolTable.add(subrRow);
        }
        return subroutineSymbolTable;
    }

    public static void pushIntegerConstant(int n, MethodVisitor mv){
        switch (n){
            case 0:
                mv.visitInsn(ICONST_0);
                break;
            case 1:
                mv.visitInsn(ICONST_1);
                break;
            case 2:
                mv.visitInsn(ICONST_2);
                break;
            case 3:
                mv.visitInsn(ICONST_3);
                break;
            case 4:
                mv.visitInsn(ICONST_4);
                break;
            case 5:
                mv.visitInsn(ICONST_5);
                break;
            default:
                mv.visitIntInsn(BIPUSH,n);
                break;
        }
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
