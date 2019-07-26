package org.vanautrui.languages;

import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.tree.*;
import org.vanautrui.languages.commandline.dragonc;
import org.vanautrui.languages.commandline.dragoni;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;

import static org.objectweb.asm.Opcodes.*;

public class App {

    //https://dzone.com/articles/generating-bytecode

    //public static final String lang_name="Dragon ";
    public static final String lang_name = "DRAGON ";

    public static void main(String[] args) {

        try_generate_some_bytecode();
        //https://www.javassist.org/
        //we could use this dependency to make our classes

        //https://www.javassist.org/tutorial/tutorial.html

        //https://www.beyondjava.net/blog/quick-guide-writing-byte-code-asm

        //https://asm.ow2.io/
        //https://asm.ow2.io/asm4-guide.pdf

        //this is the program that is actually intended to be used on the terminal:
        //for the compile part,

        //compile the source files given as arguments to java bytecode (for now)
        // dragon -c file1 file2 ...

        //compile all source files in that directory recursively
        // dragon -c directory1


        // dragon -i file1 file2 ... : interpret the given files and execute


        // dragon -i                 : just start an interpreter


        if (args.length < 1) {
            TerminalUtil.printlnRed("Dragon Programming Language: https://github.com/pointbazaar/dragon");
            TerminalUtil.printlnRed("use -i or -c argument");
            TerminalUtil.printlnRed("-i starts the Interpreter");
            TerminalUtil.printlnRed("-c starts the Compiler");
            TerminalUtil.printlnRed("");
            TerminalUtil.printlnRed("Usage: ");

            //TODO, it does not actually do that right now
            TerminalUtil.printlnRed("dragon -c file1.dragon \tcompiles file1.dragon to file1.class");
            TerminalUtil.printlnRed("dragon -i file1.dragon \tinterprets file1.dragon and prints the output to the console");

            System.exit(1);
        } else {
            String flag = args[0];

            switch (flag) {
                case "-c":
                    TerminalUtil.printlnRed(lang_name + "Compiler started", System.out);

                    dragonc.compile_main(Arrays.copyOfRange(args, 1, args.length));
                    break;
                case "-i":

                    try {

                        dragoni dragon_interpreter = new dragoni();
                        dragon_interpreter.interpret_main(Arrays.copyOfRange(args, 1, args.length));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
                default:
                    TerminalUtil.printlnRed("first flag has to be -i or -c, not " + args[0], System.out);
                    System.exit(1);
                    break;
            }
        }
    }

    private static void try_generate_some_bytecode(){

        //https://stackoverflow.com/questions/5346908/generating-a-hello-world-class-with-the-java-asm-library

        /*
        ClassNode cn = new ClassNode();

        cn.version=V1_5;
        cn.access=ACC_PUBLIC;
        cn.name="pkg/Main";
        cn.superName="java/lang/Object";

        FieldNode fieldNode=new FieldNode(ACC_PUBLIC,"field","I",null, 1);

        cn.fields.add(fieldNode);

        MethodNode methodNode = new MethodNode(ACC_PUBLIC);

        AbstractInsnNode ins = new InsnNode(ASTORE);
        methodNode.instructions.add(ins);

        cn.methods.add(methodNode);

        System.out.println(cn.toString());

         */

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
