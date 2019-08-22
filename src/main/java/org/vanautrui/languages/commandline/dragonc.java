package org.vanautrui.languages.commandline;

import org.apache.commons.cli.*;
import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

import static java.lang.System.currentTimeMillis;
import static org.objectweb.asm.Opcodes.*;
import static org.objectweb.asm.Opcodes.RETURN;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.printDuration;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.printDurationFeedback;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhases.*;

public class dragonc {
    //this should be the compiler

    //one thing to note would be that
    //we can include multiple stages of tokenization
    //and parsing.
    //parsing can include multiple passes and also
    //the AST and related data could undergo multiple representations
    //before reaching the final representation
    //from which code can be generated

    public static void compile_main(List<String> args) {
        //Apache  CLI tools is just AWESOME!!
        Options options = createOptions();
        try {
            CommandLineParser parser = new DefaultParser();
            CommandLine commandLine = parser.parse(options, args.toArray(new String[]{}));

            //TODO: provide support for compiling multiple files
            //and also for compiling a directory (recursively finds all .dragon files therein)
            Path sourceFilePath = Paths.get(args.get(0));

            //TODO: the source file must not be the first argument
            //the tool would be more flexible if the
            //source files or directory could be specified anywhere
	    if(!sourceFilePath.toString().endsWith(".dg")){
	    	throw new Exception("dragon language files should end in '.dg' for brevity and convenienc");
	    }

            compile_main_inner(sourceFilePath,commandLine);
        }catch (Exception e){
            //e.printStackTrace();
            System.err.println(e.getMessage());
            System.out.println("dgc -help     for information about command line arguments");
        }
    }

    public static void printHelp(){
        Options options = createOptions();
        HelpFormatter help = new HelpFormatter();
        StringBuilder sbh = new StringBuilder("");
        StringBuilder sbf = new StringBuilder("");

        sbh.append("\ndgc - dragon compiler\n\n");

        sbf.append("GITHUB\n");
        sbf.append("    https://github.com/pointbazaar/dragon/\n\n");

        sbf.append("AUTHOR\n");
        sbf.append(
                "    @pointbazar (alex23667@gmail.com),\n" +
                        "    @Milo-D (David.Milosevic@web.de) \n\n"
        );

        sbf.append("REPORTING BUGS\n");
        sbf.append("    https://github.com/pointbazaar/dragon/issues\n\n");

        String header=sbh.toString();
        String footer=sbf.toString();
        help.printHelp("dgc [OPTION]... FILE...",header,options,footer,true);
    }

    private static Options createOptions(){
        //https://commons.apache.org/proper/commons-cli/usage.html

        Options options = new Options();
        Option option_debug = new Option("debug",false,"provides debug output for development of the compiler");
        options.addOption(option_debug);

        options.addOption(
                new Option("timed",false,"provides a breakdown of how long each compiler phase took")
        );

        options.addOption(new Option("help",false,"display an overview of the command line options"));

        options.addOption(new Option("f","force",false,"force compilation. it will compile. (TODO)"));

        options.addOption(new Option("strict",false,"do not compile if the code is likely to have bugs (TODO)"));

        options.addOption(
                OptionBuilder
                .hasArg()
                .withArgName("LEVEL")
                .withDescription("try to optimize the code. optimization effort goes from 0 (no optimization) to 10 (maximum optimization")
                .create("optimize")
        );


        OptionGroup optionGroup = new OptionGroup();

        optionGroup.addOption(new Option("nocurly",false,"accept only indentation for scopes (TODO)"));
        optionGroup.addOption(new Option("curly",false,"accept only curly braces  for scopes (TODO)"));



        options.addOptionGroup(optionGroup);
        return options;
    }

    private static void compile_main_inner(Path sourceFilePath,CommandLine cmd){

        if(cmd.hasOption("help")){
            printHelp();
            return;
        }

        boolean debug=cmd.hasOption("debug");
        boolean timed=cmd.hasOption("timed");

        long start_time_ms = currentTimeMillis();

        //TODO: expand functionality to directories and multiple files
        try {

            String sourceCode = new String(Files.readAllBytes(sourceFilePath));

            if(debug) {
                System.out.println(sourceCode);
            }

            long start,end;

            start = currentTimeMillis();
            //PHASE CLEAN
            String codeWithoutCommentsWithoutUnneccesaryWhitespace
                    = phase_clean(sourceCode,debug);
            end= currentTimeMillis();
            if(timed) {
                printDuration(start, end);
            }

            String just_code_with_braces_without_comments_without_newlines;

            if(cmd.hasOption("nocurly")){
                just_code_with_braces_without_comments_without_newlines
                        =phase_conditional_weave_curly_braces(codeWithoutCommentsWithoutUnneccesaryWhitespace,debug);
            }else{
                //the editor is curly by default
                just_code_with_braces_without_comments_without_newlines
                        =codeWithoutCommentsWithoutUnneccesaryWhitespace;
            }

            start= currentTimeMillis();
            //PHASE LEXING
            TokenList tokens = phase_lexing(just_code_with_braces_without_comments_without_newlines,debug);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE PARSING
            Set<AST> asts = phase_parsing(tokens,debug);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE TYPE CHECKING
            phase_typecheck(asts,debug);
            end = currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start = currentTimeMillis();
            //PHASE CODE GENERATION
            phase_codegeneration(asts,debug);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            long end_time_ms = currentTimeMillis();
            long duration = end_time_ms-start_time_ms;

            //https://www.utf8icons.com/

            printDurationFeedback(duration);

        } catch (Exception e) {

            System.err.println(e.getMessage());
            if(cmd.hasOption("debug")) {
                //only print the stack trace for
                //compiler developers
                e.printStackTrace();
            }
        }
    }

    private static void try_generate_some_bytecode(){
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
