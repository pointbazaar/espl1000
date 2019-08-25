package org.vanautrui.languages.commandline.compilerphases;

import org.apache.commons.cli.CommandLine;
import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.codegeneration.JavaByteCodeGenerator;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.lexing.Lexer;
import org.vanautrui.languages.lexing.utils.CurlyBracesWeaver;
import org.vanautrui.languages.parsing.Parser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.phase_clean_the_input.CommentRemover;
import org.vanautrui.languages.typechecking.TypeChecker;

import static org.fusesource.jansi.Ansi.ansi;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.printEndPhase;
import static org.vanautrui.languages.symboltablegenerator.SymbolTableGenerator.*;

import com.fasterxml.jackson.databind.*;

import java.io.File;
import java.util.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashSet;
import java.util.Set;

import static org.fusesource.jansi.Ansi.Color.GREEN;
import static org.fusesource.jansi.Ansi.Color.RED;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.printBeginPhase;
import static org.vanautrui.languages.phase_clean_the_input.UnneccessaryWhiteSpaceRemover.remove_unneccessary_whitespace;

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
        List<String> results=new ArrayList();
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

    public static void phase_typecheck(Set<AST> asts, CommandLine cmd)throws Exception{
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

    public static void phase_codegeneration(Set<AST> asts, CommandLine cmd)throws Exception{
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("CODE GENERATION",printLong);

        //printBeginPhase("CODE GENERATION");
        final boolean debug=cmd.hasOption("debug");

        try {
            for(AST ast : asts) {
                for (ClassNode classNode : ast.classNodeList) {

                    //TODO: create the symbol table with all classes in mind, not just this one
                    SubroutineSymbolTable subroutineSymbolTable = createSubroutineSymbolTable(classNode);

                    //generate bytecode for that class
                    byte[] classResult = JavaByteCodeGenerator.generateByteCodeForClass(classNode, subroutineSymbolTable, debug);
                    Files.write(Paths.get(classNode.name.typeName + ".class"), classResult);
                }
            }
            printEndPhase(true,printLong);
            //TerminalUtil.println("✓", Ansi.Color.GREEN);

        }catch (Exception e){
            printEndPhase(false,printLong);
            //TerminalUtil.println("⚠", RED);
            throw e;
        }
    }

    private static Path makeCleanPhaseCacheFilePathFromHash(int hash){
        final String extension = ".dragon.cleaned";
        //hidden file. important, so that it does not be visible and bother people
        return Paths.get(phase_clean_cache_dir+"."+hash+extension);
    }

    private static final String phase_clean_cache_dir=System.getProperty("user.home")+"/dragoncache/clean/";

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
            results.add(new CharacterList(codeWithoutCommentsWithoutUnneccesaryWhitespace,sourceFiles.get(i)));
        }

        //TerminalUtil.println("✓", Ansi.Color.GREEN);
        printEndPhase(true,printLong);
        

        return results;
    }

    public static Set<AST> phase_parsing(List<TokenList> list, CommandLine cmd)throws Exception{
        final boolean debug=cmd.hasOption("debug");
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("PARSING",printLong);
        HashSet<AST> asts=new HashSet<>();

        try {
            for(TokenList tokens : list){
                AST ast = (new Parser()).parse(tokens);

                if(debug){
                    TerminalUtil.println("DEBUG: TODO: pretty print source from AST in curly braces", RED);
                    System.out.println(ast.toSourceCode());

                    TerminalUtil.println("DEBUG: PRINT AST JSON ", RED);

                    //Serializer serializer = new Persister(getPreferredXMLSerializationStrategyHumanReadable());
                    //serializer.write(ast, System.out);
    				ObjectMapper mapper = new ObjectMapper();
    				mapper.enable(SerializationFeature.INDENT_OUTPUT);
    				System.out.println(mapper.writeValueAsString(ast));
                    System.out.println();
                }
                
                asts.add(ast);
            }
            printEndPhase(true,printLong);
            return asts;
        }catch (Exception e){
            //TerminalUtil.println("⚠",RED);
            printEndPhase(false,printLong);
            throw e;
        }
    }

    public static List<TokenList> phase_lexing(List<CharacterList> just_codes_with_braces_without_comments, CommandLine cmd)throws Exception{
        final boolean debug=cmd.hasOption("debug");
        final boolean printLong = cmd.hasOption("debug")||cmd.hasOption("timed");
        printBeginPhase("LEXING",printLong);
        List<TokenList> list=new ArrayList();
        try{
            for(CharacterList just_code_with_braces_without_comments: just_codes_with_braces_without_comments){

                TokenList tokens = (new Lexer()).lexCodeWithoutComments(just_code_with_braces_without_comments);

                if(debug) {
                    System.out.println(tokens.toString());
                }
                list.add(tokens);
            }
            printEndPhase(true,printLong);
            return list;
        }catch (Exception e){
            printEndPhase(false,printLong);
            throw e;
        }
    }

}
