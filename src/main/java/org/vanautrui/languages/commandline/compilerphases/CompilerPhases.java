package org.vanautrui.languages.commandline.compilerphases;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import org.apache.commons.cli.CommandLine;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.codegeneration.jvmbackend.JavaByteCodeGenerator;
import org.vanautrui.languages.lexing.Lexer;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.utils.CurlyBracesWeaver;
import org.vanautrui.languages.parsing.Parser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.phase_clean_the_input.CommentRemover;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.typechecking.TypeChecker;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

import static org.fusesource.jansi.Ansi.Color.RED;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.printBeginPhase;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.printEndPhase;
import static org.vanautrui.languages.phase_clean_the_input.UnneccessaryWhiteSpaceRemover.remove_unneccessary_whitespace;
import static org.vanautrui.languages.symboltablegenerator.SymbolTableGenerator.createSubroutineSymbolTable;

public class CompilerPhases {

    public static List<String> phase_conditional_weave_curly_braces(List<String> codesWithoutCommentsWithoutUnneccesaryWhitespace, CommandLine cmd) throws Exception {
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("PHASE: WEAVE IN CURLY BRACES",printLong);

        //TODO: put the semicolons in?
        //the tokens should know which line number they are at.
        //at the end of each line which is not opening '{' or closing '}' a scope
        //  with that being defined as '{' being the last token on the line
        //  and '}' being the first token on the line respectively
        //, there should be a ';' , a semicolon.

        //so we can identify the different statements.
        //but we cannot exactly do it like this because it would
        //prevent people from making a multi-line statement
        //let us abandon this approach for now.

        //i have an idea how we can avoid an issue related to this
        final boolean debug=cmd.hasOption("debug");
        List<String> results=new ArrayList<>();
        try {
            for(String codeWithoutCommentsWithoutUnneccesaryWhitespace: codesWithoutCommentsWithoutUnneccesaryWhitespace){
                String just_code_with_braces_without_comments_without_newlines =
                        CurlyBracesWeaver
                                .weave_scoping_curly_braces_and_remove_newlines(codeWithoutCommentsWithoutUnneccesaryWhitespace);

                if(debug) {
                    System.out.println(just_code_with_braces_without_comments_without_newlines);
                }
                results.add(just_code_with_braces_without_comments_without_newlines);
            }
            printEndPhase(true,printLong);
            return results;
        }catch (Exception e){
            //TerminalUtil.println("⚠", RED);
            printEndPhase(false,printLong);
            throw e;
        }
    }

    public static void phase_typecheck(List<AST> asts, CommandLine cmd)throws Exception{
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("TYPE CHECKING",printLong);

        //this should throw an exception, if it does not typecheck
        try {
            TypeChecker typeChecker=new TypeChecker();
            typeChecker.doTypeCheck(asts);

            //TerminalUtil.println("✓", Ansi.Color.GREEN);
            printEndPhase(true,printLong);
        }catch (Exception e){
            //TerminalUtil.println("⚠", RED);
            printEndPhase(true,printLong);
            throw e;
        }
    }

    public static void phase_run_optional(List<Path> classFilePaths,CommandLine cmd)throws Exception{

        for(Path p: classFilePaths){
            String command = "java -classpath "+p.toAbsolutePath().getParent()+" "+p.getFileName().toString().split("\\.")[0];
            if(cmd.hasOption("debug")){
                System.out.println("trying to execute: "+p.getFileName());
                System.out.println("command: "+command);
            }

            Process pr = Runtime.getRuntime().exec(command);
            //TODO: connect input and output to this process's input and output

            InputStream out = pr.getInputStream();
            OutputStream in = pr.getOutputStream();

            Scanner sc = new Scanner(System.in);

            //PipedInputStream pis = new PipedInputStream();
            //PipedOutputStream pos = new PipedOutputStream(in);
            (new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        while (sc.hasNextLine()) {
                            System.out.println("//has next line from user");
                            in.write(sc.nextLine().getBytes());
                        }
                        in.close();
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                }
            })).start();

            (new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        char c;
                        while ((c = (char) out.read()) != -1) {
                            System.out.println("//read from process");
                            System.out.print(c);
                        }
                        System.out.close();
                    }catch (Exception e){
                        e.printStackTrace();
                        System.out.close();
                    }
                }
            })).start();

            /*
            Scanner scanner = new Scanner(System.in);

            while(
                    (out.available()>0 || scanner.hasNext()) && pr.isAlive()
            ){
                System.out.println("//loop iteration");
                if(out.available()>0){
                    System.out.println("// out available: "+out.available());

                    int read = out.read();
                    System.out.print((char)read);
                }else if(scanner.hasNextByte()){
                    System.out.println("// scanner.hasNextByte()==true");
                    in.write(scanner.nextByte());
                    in.flush();
                }
            }

            pr.waitFor();
            int exitValue = pr.exitValue();

            if(exitValue!=0){
                throw new Exception(IOUtils.toString(pr.getErrorStream()));
            }

             */

            //for now, just display the output
            //String out = IOUtils.toString(pr.getInputStream());
            //System.out.println(out);
        }
    }

    public static List<Path> phase_codegeneration(List<AST> asts, CommandLine cmd)throws Exception{
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("CODE GENERATION",printLong);

        List<Path> classFilePaths=new ArrayList<>();
        final boolean debug=cmd.hasOption("debug");

        try {
            for(AST ast : asts) {
                for (ClassNode classNode : ast.classNodeList) {

                    //TODO: create the symbol table with all classes in mind, not just this one
                    SubroutineSymbolTable subroutineSymbolTable = createSubroutineSymbolTable(classNode);

                    if(debug || cmd.hasOption("symboltables")){
                        System.out.println(subroutineSymbolTable.toString());
                    }

                    //generate bytecode for that class
                    byte[] classResult = JavaByteCodeGenerator.generateByteCodeForClass(classNode, subroutineSymbolTable, debug);

                    //System.out.println(ast.srcPath.toAbsolutePath().getParent());
                    String dir=ast.srcPath.toAbsolutePath().getParent().toString();
                    Path classFilePath = Paths.get(dir + "/" + classNode.name.typeName + ".class");
                    Files.write(classFilePath, classResult);
                    classFilePaths.add(classFilePath);
                }
            }
            printEndPhase(true,printLong);
            return classFilePaths;

        }catch (Exception e){
            printEndPhase(false,printLong);
            throw e;
        }
    }

    private static Path makeCleanPhaseCacheFilePathFromHash(int hash){
        final String extension = ".dragon.cleaned";
        //hidden file. important, so that it does not be visible and bother people
        return Paths.get(phase_clean_cache_dir+"."+hash+extension);
    }

    private static final String phase_clean_cache_dir=System.getProperty("user.home")+"/.dragoncache/clean/";

    public static List<CharacterList> phase_clean(List<String> sources, List<File> sourceFiles, CommandLine cmd)throws Exception{
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("CLEAN",printLong);
        //(remove comments, empty lines, excess whitespace)
        List<CharacterList> results=new ArrayList();

        for(int i=0;i<sources.size();i++){
            String source=sources.get(i);
            if(!Files.exists(Paths.get(phase_clean_cache_dir))){
                Files.createDirectories(Paths.get(phase_clean_cache_dir));
            }
            final boolean debug=cmd.hasOption("debug");

            int hash = source.hashCode();
            if(debug) {
                System.out.println("phase clean: Hashcode of source string: " + hash);
            }
            boolean foundCachedCleanedFile = false;

            if(Files.exists(makeCleanPhaseCacheFilePathFromHash(hash))){
                foundCachedCleanedFile=true;
            }

            String codeWithoutCommentsWithoutUnneccesaryWhitespace;

            if(foundCachedCleanedFile){
                if(debug) {
                    System.out.println("found a cached version that is already cleaned");
                }
                codeWithoutCommentsWithoutUnneccesaryWhitespace = new String(Files.readAllBytes(makeCleanPhaseCacheFilePathFromHash(hash)));
            }else {

                String codeWithoutCommentsAndWithoutEmptyLines = (new CommentRemover()).strip_all_comments_and_empty_lines(source);

                codeWithoutCommentsWithoutUnneccesaryWhitespace =
                        remove_unneccessary_whitespace(codeWithoutCommentsAndWithoutEmptyLines);

                //write file for caching
                Files.write(makeCleanPhaseCacheFilePathFromHash(hash),codeWithoutCommentsWithoutUnneccesaryWhitespace.getBytes());
            }
            if(debug) {
                //System.out.println(codeWithoutCommentsAndWithoutEmptyLines);
                System.out.println(codeWithoutCommentsWithoutUnneccesaryWhitespace);
            }
            results.add(new CharacterList(codeWithoutCommentsWithoutUnneccesaryWhitespace,sourceFiles.get(i).toPath()));
        }

        //TerminalUtil.println("✓", Ansi.Color.GREEN);
        printEndPhase(true,printLong);
        

        return results;
    }

    public static List<AST> phase_parsing(List<TokenList> list, CommandLine cmd)throws Exception{
        final boolean debug=cmd.hasOption("debug");
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("PARSING",printLong);
        List<AST> asts=new ArrayList<>();
        boolean didThrow=false;
        List<Exception> exceptions=new ArrayList<>();

        for(TokenList tokens : list){
            try {
                AST ast = (new Parser()).parse(tokens,tokens.relPath);

                if (debug || cmd.hasOption("ast")) {
                    //TerminalUtil.println("DEBUG: pretty print source from AST in curly braces", RED);
                    //System.out.println(ast.toSourceCode());

                    TerminalUtil.println("\nDEBUG: PRINT AST JSON ", RED);

                    ObjectMapper mapper = new ObjectMapper();
                    mapper.enable(SerializationFeature.INDENT_OUTPUT);
                    System.out.println(mapper.writeValueAsString(ast));
                    System.out.println();
                }

                asts.add(ast);
            }catch (Exception e) {
                didThrow=true;
                exceptions.add(e);
            }
        }

        if(didThrow){
            printEndPhase(false,printLong);
            throw new Exception(exceptions.stream().map(e->e.getMessage()).collect(Collectors.joining("\n")));
        }else{
            printEndPhase(true,printLong);
            return asts;
        }

    }

    public static List<TokenList> phase_lexing(List<CharacterList> just_codes_with_braces_without_comments, CommandLine cmd)throws Exception{
        final boolean debug=cmd.hasOption("debug");
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("LEXING",printLong);
        List<TokenList> list=new ArrayList();
        boolean didThrow = false;
        List<Exception> exceptions=new ArrayList<>();

        for(CharacterList just_code_with_braces_without_comments: just_codes_with_braces_without_comments){
            try {
                TokenList tokens = (new Lexer()).lexCodeWithoutComments(just_code_with_braces_without_comments);

                if (debug || cmd.hasOption("tokens")) {
                    System.out.println(tokens.toString());
                }
                list.add(tokens);
            }catch (Exception e){
                exceptions.add(e);
                didThrow=true;
            }
        }

        if(didThrow){
            printEndPhase(false,printLong);
            //collect all the exceptions throw during lexing,
            //and combine their messages to throw a bigger exception
            throw new Exception(exceptions.stream().map(Throwable::getMessage).collect(Collectors.joining("\n")));
        }else {
            printEndPhase(true,printLong);
            return list;
        }
    }

}
