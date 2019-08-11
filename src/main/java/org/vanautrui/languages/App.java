package org.vanautrui.languages;

import org.apache.commons.cli.*;
import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.commandline.dragonc;
import org.vanautrui.languages.editor.gui.DragonGUI_Editor;
import org.vanautrui.languages.commandline.dragoni;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Optional;

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

        Options options = createOptions();
        CommandLineParser parser = new DefaultParser();

        HelpFormatter helpFormatter = new HelpFormatter();
        try {
            CommandLine cmd = parser.parse(options, args);

            if(cmd.hasOption("c")){
                dragonc.compile_main(Arrays.copyOfRange(args, 1, args.length));
            }else if(cmd.hasOption("i")){
                dragoni dragon_interpreter = new dragoni();
                dragon_interpreter.interpret_main(cmd.getArgList().subList(1,cmd.getArgList().size()));
            }else if(cmd.hasOption("e")){
                Optional<Path> filePath = Optional.empty();
                try {
                    filePath = Optional.of(Paths.get(cmd.getArgList().get(1)));
                }catch (Exception e){
                    //pass
                }
                DragonGUI_Editor editor = new DragonGUI_Editor(filePath);
            }
        }catch (Exception e){
            System.out.println(e.getMessage());
            helpFormatter.printHelp("dragon ",options);

            //e.printStackTrace();
        }
    }

    private static Options createOptions(){
        Option compile_flag = OptionBuilder.withDescription("use the compiler").isRequired(false).create("c");
        Option interpreter_flag = OptionBuilder.withDescription("use the interpreter").isRequired(false).create("i");
        Option editor_flag = OptionBuilder.withDescription("start the ide").isRequired(false).create("e");

        OptionGroup flag_group = new OptionGroup();
        flag_group.addOption(compile_flag);
        flag_group.addOption(interpreter_flag);
        flag_group.addOption(editor_flag);

        flag_group.setRequired(true);

        Options options = new Options();
        options.addOptionGroup(flag_group);
        return options;
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
