package org.vanautrui.languages;

import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.commandline.dragonc;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;

import static org.objectweb.asm.Opcodes.*;

public class App {

    //https://dzone.com/articles/generating-bytecode

    //public static final String lang_name="Dragon ";
    public static final String lang_name = "DRAGON ";

    public static void main(String[] args) {
        //https://www.javassist.org/
        //we could use this dependency to make our classes

        //https://www.javassist.org/tutorial/tutorial.html
        //https://www.beyondjava.net/blog/quick-guide-writing-byte-code-asm
        //https://asm.ow2.io/
        //https://asm.ow2.io/asm4-guide.pdf

        try {
            dragonc.compile_main(Arrays.asList(args));
            System.exit(0);
        }catch (Exception e){
            //e.printStackTrace();
            System.err.println(e.getMessage());
            System.out.println("-help     for information about command line arguments");
            System.exit(1);
        }
    }

    private static void try_generate_some_bytecode(){

        //https://stackoverflow.com/questions/5346908/generating-a-hello-world-class-with-the-java-asm-library

        //IMPORTANT: there are intellij plugins for viewing bytecode and such.

        //copied from https://stackoverflow.com/questions/5346908/generating-a-hello-world-class-with-the-java-asm-library

        ClassWriter cw = new ClassWriter(0);
        FieldVisitor fv;
        MethodVisitor mv;
        AnnotationVisitor av0;

        cw.visit(49,
                ACC_PUBLIC + ACC_SUPER,
                "Hello",
                null,
                "java/lang/Object",
                null);

        cw.visitSource("Hello.java", null);

        {
            mv = cw.visitMethod(ACC_PUBLIC, "<init>", "()V", null, null);
            mv.visitVarInsn(ALOAD, 0);
            mv.visitMethodInsn(INVOKESPECIAL,
                    "java/lang/Object",
                    "<init>",
                    "()V");
            mv.visitInsn(RETURN);
            mv.visitMaxs(1, 1);
            mv.visitEnd();
        }
        {
            mv = cw.visitMethod(ACC_PUBLIC + ACC_STATIC,
                    "main",
                    "([Ljava/lang/String;)V",
                    null,
                    null);
            mv.visitFieldInsn(GETSTATIC,
                    "java/lang/System",
                    "out",
                    "Ljava/io/PrintStream;");
            mv.visitLdcInsn("hello");
            mv.visitMethodInsn(INVOKEVIRTUAL,
                    "java/io/PrintStream",
                    "println",
                    "(Ljava/lang/String;)V");
            mv.visitInsn(RETURN);
            mv.visitMaxs(2, 1);
            mv.visitEnd();
        }
        cw.visitEnd();

        byte[] mybytecode = cw.toByteArray();

        try {
            Files.write(Paths.get("Hello.class"), mybytecode);
        }catch (Exception e){
            e.printStackTrace();
        }

    }
}
