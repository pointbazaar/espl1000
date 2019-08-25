package org.vanautrui.languages.commandline;

import org.apache.commons.cli.*;
import org.apache.commons.io.*;
import org.apache.commons.io.filefilter.RegexFileFilter;
import org.fusesource.jansi.Ansi;
import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

import static java.lang.System.currentTimeMillis;
import static org.fusesource.jansi.Ansi.ansi;
import static org.objectweb.asm.Opcodes.*;
import static org.objectweb.asm.Opcodes.RETURN;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.*;
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
            CommandLine cmd = parser.parse(options, args.toArray(new String[]{}));

            //TODO: provide support for compiling multiple files
            //and also for compiling a directory (recursively finds all .dragon files therein)

            //as no option currently has an argument,
            //this simplifies the usage of the compiler
            //also, everything that doesnt start with '-' is either a source file or directory

            if(cmd.hasOption("clean")){
                if(cmd.hasOption("debug")){
                    System.out.println("clearing the cache");
                }
                final String cache_dir=System.getProperty("user.home")+"/dragoncache";
                FileUtils.deleteDirectory(Paths.get(cache_dir).toFile());
            }
            List<String> fileArgs = args.stream().filter(str->!str.startsWith("-")).collect(Collectors.toList());

            compile_main_inner(getAllDragonFilesRecursively(fileArgs),cmd);
        }catch (Exception e){
            //e.printStackTrace();
            System.err.println(e.getMessage());
            System.out.println("dgc -help     for information about command line arguments");
        }
    }

    private static List<File> getAllDragonFilesRecursively(List<String> fileArgs)throws Exception{
        //from dgc options, this can either be files or directories
        //which must be compiled together.
        List<File> results=new ArrayList<>();
        for(String s : fileArgs) {
            Path path = Paths.get(s);
            if(path.toFile().isDirectory()) {
                //add all the files recursively
                Collection<File> files = FileUtils.listFiles(
                        path.toFile(),
                        new String[]{"dg"},
                        true
                );
                results.addAll(files);
            }else {
                if (s.endsWith(".dg")) {
                    results.add(path.toFile());
                }
            }
        }
        if(results.size()==0){
            throw new Exception("could not find any files with '.dg' extension.");
        }
        return results;
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

        options.addOption(new Option("force",false,"force compilation. it will compile. (TODO)"));

        options.addOption(new Option("strict",false,"do not compile if the code is likely to have bugs (TODO)"));

        options.addOption(new Option("clean",false,"clears the cache"));

        options.addOption(
                new Option(
                    "optimize",
                    false,
                    "try to optimize the code. optimization effort goes from 0 (no optimization) to 10 (maximum optimization"
                    )
        );


        OptionGroup optionGroup = new OptionGroup();

        optionGroup.addOption(new Option("nocurly",false,"accept only indentation for scopes (TODO)"));
        optionGroup.addOption(new Option("curly",false,"accept only curly braces  for scopes (TODO)"));



        options.addOptionGroup(optionGroup);
        return options;
    }

    private static void compile_main_inner(List<File> sources,CommandLine cmd){

        if(cmd.hasOption("help")){
            printHelp();
            return;
        }

        boolean debug=cmd.hasOption("debug");
        boolean timed=cmd.hasOption("timed");

        long start_time_ms = currentTimeMillis();

        //TODO: expand functionality to directories and multiple files
        try {
            List<String> codes=new ArrayList();
            for(File file : sources){
                String sourceCode = new String(Files.readAllBytes(file.toPath()));
                codes.add(sourceCode);
                if(debug) {
                    System.out.println(sourceCode);
                }
            }

            long start,end;

            start = currentTimeMillis();
            //PHASE CLEAN
            List<String> codeWithoutCommentsWithoutUnneccesaryWhitespace
                    = phase_clean(codes,cmd);

            end= currentTimeMillis();
            if(timed) {
                printDuration(start, end);
            }

            List<String> just_code_with_braces_without_comments_without_newlines;

            if(cmd.hasOption("nocurly")){
                just_code_with_braces_without_comments_without_newlines
                        =phase_conditional_weave_curly_braces(codeWithoutCommentsWithoutUnneccesaryWhitespace,cmd);
            }else{
                //the editor is curly by default
                just_code_with_braces_without_comments_without_newlines
                        =codeWithoutCommentsWithoutUnneccesaryWhitespace;
            }

            start= currentTimeMillis();
            //PHASE LEXING
            List<TokenList> tokens = phase_lexing(just_code_with_braces_without_comments_without_newlines,cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE PARSING
            Set<AST> asts = phase_parsing(tokens,cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE TYPE CHECKING
            phase_typecheck(asts,cmd);
            end = currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start = currentTimeMillis();
            //PHASE CODE GENERATION
            phase_codegeneration(asts,cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            long end_time_ms = currentTimeMillis();
            long duration = end_time_ms-start_time_ms;

            //https://www.utf8icons.com/
            if(timed) {
                printDurationFeedback(duration);
            }
            {
                //System.out.println();
                Ansi ansi1= ansi();
                ansi1.cursorToColumn(0);
                ansi1.eraseLine(Ansi.Erase.FORWARD);
                ansi1.reset();
                System.out.print(ansi1);
                System.out.flush();
                printBuildConclusion(true);

            }

        } catch (Exception e) {

            System.err.println(e.getMessage());
            if(cmd.hasOption("debug")) {
                //only print the stack trace for
                //compiler developers
                e.printStackTrace();
            }
            printBuildConclusion(false);
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
