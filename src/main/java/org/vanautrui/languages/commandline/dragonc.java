package org.vanautrui.languages.commandline;

import org.apache.commons.cli.*;
import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;
import org.simpleframework.xml.strategy.Strategy;
import org.simpleframework.xml.strategy.Type;
import org.simpleframework.xml.strategy.Visitor;
import org.simpleframework.xml.strategy.VisitorStrategy;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.NodeMap;
import org.simpleframework.xml.stream.OutputNode;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.codegeneration.JavaByteCodeGenerator;
import org.vanautrui.languages.lexing.utils.CurlyBracesWeaver;
import org.vanautrui.languages.lexing.utils.DragonCommentRemover;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.parsing.DragonParser;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import static org.objectweb.asm.Opcodes.*;
import static org.objectweb.asm.Opcodes.RETURN;
import static org.vanautrui.languages.commandline.DragonCompilerPhases.*;

public class dragonc {
    //this should be the compiler

    //one thing to note would be that
    //we can include multiple stages of tokenization
    //and parsing.
    //parsing can include multiple passes and also
    //the AST and related data could undergo multiple representations
    //before reaching the final representation
    //from which code can be generated

    public static void compile_main(String[] args) {
        //Apache  CLI tools is just AWESOME!!
        Options options = createOptions();
        try {
            CommandLineParser parser = new DefaultParser();
            CommandLine commandLine = parser.parse(options, args);

            //TODO: provide support for compiling multiple files
            //and also for compiling a directory (recursively finds all .dragon files therein)
            Path sourceFilePath = Paths.get(args[0]);

            //TODO: the source file must not be the first argument
            //the tool would be more flexible if the
            //source files or directory could be specified anywhere

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
        Option option_debug = new Option("d","debug",false,"provides debug output for development of the compiler");
        options.addOption(option_debug);

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

        //TODO: expand functionality to directories and multiple files
        try {

            String sourceCode = new String(Files.readAllBytes(sourceFilePath));

            if(cmd.hasOption("debug")) {
                System.out.println(sourceCode);
            }

            String codeWithoutCommentsWithoutUnneccesaryWhitespace
                    = phase_clean(sourceCode,cmd.hasOption("debug"));

            String just_code_with_braces_without_comments_without_newlines;

            if(cmd.hasOption("nocurly")){
                just_code_with_braces_without_comments_without_newlines
                        =phase_conditional_weave_curly_braces(codeWithoutCommentsWithoutUnneccesaryWhitespace,cmd.hasOption("debug"));
            }else{
                //the editor is curly by default
                just_code_with_braces_without_comments_without_newlines
                        =codeWithoutCommentsWithoutUnneccesaryWhitespace;
            }

            DragonTokenList tokens = phase_lexing(just_code_with_braces_without_comments_without_newlines,cmd.hasOption("debug"));

            DragonAST ast = phase_parsing(tokens,cmd.hasOption("debug"));

            Set<DragonAST> asts = new HashSet<>();
            asts.add(ast);

            phase_typecheck(asts,ast,cmd.hasOption("debug"));

            phase_codegeneration(ast,cmd.hasOption("debug"));

        } catch (Exception e) {

            System.err.println(e.getMessage());
            if(cmd.hasOption("debug")) {
                //only print the stack trace for
                //compiler developers
                e.printStackTrace();
            }
        }
    }



    public static final Strategy getPreferredXMLSerializationStrategyHumanReadable(){
        Strategy strategy = new VisitorStrategy(new Visitor() {
            @Override
            public void read(Type type, NodeMap<InputNode> nodeMap) throws Exception {

            }

            @Override
            public void write(Type type, NodeMap<OutputNode> nodeMap) throws Exception {
                nodeMap.remove("class");
            }
        });
        return strategy;
    };

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

    public static String remove_unneccessary_whitespace(String codeWithoutCommentsAndWithoutEmptyLines) {

        String result = Arrays.stream(codeWithoutCommentsAndWithoutEmptyLines.split("\n"))
                .map(
                        str->remove_unneccessary_whitespace_line(str)
                )
                .collect(Collectors.joining("\n"));

        return result;
    }

    private static String remove_unneccessary_whitespace_line(String str) {
        //the initial whitespace, and whitespace within strings should remain
        //1 whitespace between words should remain

        StringBuilder sb=new StringBuilder("");

        //TODO: save the indentation from being removed

        boolean last_was_whitespace=true;
        boolean inside_string=false;
        boolean start_of_line=false;

        int i=0;

        while(i<str.length() && !start_of_line){
            char c = str.charAt(i);
            switch (c) {
                case '\t':
                case ' ':
                    sb.append(c);
                    i++;
                    break;
                default:
                    start_of_line = true;
            }
        }

        while(i<str.length()){
            char c=str.charAt(i);

            switch (c){
                case '\"':
                    inside_string=!inside_string;
                    last_was_whitespace=false;
                    sb.append(c);
                    break;
                case ' ':
                    if(last_was_whitespace && !inside_string){
                        //we skip it
                    }else{
                        sb.append(c);
                    }
                    last_was_whitespace=true;
                    break;
                default:
                    last_was_whitespace=false;
                    sb.append(c);
                    break;
            }
            i++;
        }

        return sb.toString();
    }
}