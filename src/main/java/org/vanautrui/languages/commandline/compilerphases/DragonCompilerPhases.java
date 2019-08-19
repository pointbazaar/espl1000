package org.vanautrui.languages.commandline.compilerphases;

import org.fusesource.jansi.Ansi;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.codegeneration.JavaByteCodeGenerator;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.utils.CurlyBracesWeaver;
import org.vanautrui.languages.parsing.DragonParser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.phase_clean_the_input.DragonCommentRemover;
import org.vanautrui.languages.typechecking.DragonTypeChecker;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashSet;
import java.util.Set;

import static org.fusesource.jansi.Ansi.Color.GREEN;
import static org.fusesource.jansi.Ansi.Color.RED;
import static org.vanautrui.languages.codegeneration.JavaByteCodeGenerator.createSubroutineSymbolTable;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.getPreferredXMLSerializationStrategyHumanReadable;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.printBeginPhase;
import static org.vanautrui.languages.phase_clean_the_input.DragonUnneccessaryWhiteSpaceRemover.remove_unneccessary_whitespace;

public class DragonCompilerPhases {

    public static String phase_conditional_weave_curly_braces(String codeWithoutCommentsWithoutUnneccesaryWhitespace, boolean debug) throws Exception {
        printBeginPhase("PHASE: WEAVE IN CURLY BRACES");

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


        try {
            String just_code_with_braces_without_comments_without_newlines =
                    CurlyBracesWeaver
                            .weave_scoping_curly_braces_and_remove_newlines(codeWithoutCommentsWithoutUnneccesaryWhitespace);

            TerminalUtil.println("✓", Ansi.Color.GREEN);

            if(debug) {
                System.out.println(just_code_with_braces_without_comments_without_newlines);
            }

            return just_code_with_braces_without_comments_without_newlines;
        }catch (Exception e){
            TerminalUtil.println("⚠", RED);
            throw e;
        }
    }

    public static void phase_typecheck(Set<DragonAST> asts, boolean debug)throws Exception{

        printBeginPhase("TYPE CHECKING");

        //this should throw an exception, if it does not typecheck
        try {
            DragonTypeChecker typeChecker=new DragonTypeChecker();
            typeChecker.doTypeCheck(asts);

            TerminalUtil.println("✓", Ansi.Color.GREEN);
        }catch (Exception e){
            TerminalUtil.println("⚠", RED);
            throw e;
        }
    }

    public static void phase_codegeneration(Set<DragonAST> asts, boolean debug)throws Exception{
        printBeginPhase("CODE GENERATION");


        try {
            for(DragonAST ast : asts) {
                for (DragonClassNode classNode : ast.classNodeList) {

                    //TODO: create the symbol table with all classes in mind, not just this one
                    DragonSubroutineSymbolTable subroutineSymbolTable = createSubroutineSymbolTable(classNode);

                    //generate bytecode for that class
                    byte[] classResult = JavaByteCodeGenerator.generateByteCodeForClass(classNode, subroutineSymbolTable, debug);
                    Files.write(Paths.get(classNode.name.typeName.getContents() + ".class"), classResult);
                }
            }
            TerminalUtil.println("✓", Ansi.Color.GREEN);
        }catch (Exception e){
            TerminalUtil.println("⚠", RED);
            throw e;
        }
    }

    public static String phase_clean(String source, boolean debug)throws Exception{
        printBeginPhase("CLEAN");
        //(remove comments, empty lines, excess whitespace)

        //TerminalUtil.printlnRed("PHASE: REMOVE COMMENTS AND EMPTY LINES");

        String codeWithoutCommentsAndWithoutEmptyLines = (new DragonCommentRemover()).strip_all_comments_and_empty_lines(source);

        //maybe phase to remove unneccessary whitespace?
        //TerminalUtil.printlnRed("PHASE: REMOVE UNNECCESSARY WHITESPACE");

        String codeWithoutCommentsWithoutUnneccesaryWhitespace =
                remove_unneccessary_whitespace(codeWithoutCommentsAndWithoutEmptyLines);

        TerminalUtil.println("✓", Ansi.Color.GREEN);

        if(debug) {
            System.out.println(codeWithoutCommentsAndWithoutEmptyLines);
            System.out.println(codeWithoutCommentsWithoutUnneccesaryWhitespace);
        }

        return codeWithoutCommentsWithoutUnneccesaryWhitespace;
    }

    public static Set<DragonAST> phase_parsing(DragonTokenList tokens, boolean debug)throws Exception{
        printBeginPhase("PARSING");
        try {
            DragonAST ast = (new DragonParser()).parse(tokens);
            TerminalUtil.println("✓", Ansi.Color.GREEN);

            if(debug){
                TerminalUtil.println("DEBUG: TODO: pretty print source from AST in curly braces", RED);
                System.out.println(ast.toSourceCode());

                TerminalUtil.println("DEBUG: PRINT AST XML ", RED);

                Serializer serializer = new Persister(getPreferredXMLSerializationStrategyHumanReadable());
                serializer.write(ast, System.out);
                System.out.println();
            }
            HashSet<DragonAST> asts=new HashSet<>();
            asts.add(ast);
            return asts;
        }catch (Exception e){
            TerminalUtil.println("⚠",RED);
            throw e;
        }
    }

    public static DragonTokenList phase_lexing(String just_code_with_braces_without_comments, boolean debug)throws Exception{
        printBeginPhase("LEXING");

        String just_code_with_braces_without_comments_without_newlines = just_code_with_braces_without_comments.replaceAll("\n","");

        try {
            DragonTokenList tokens = (new DragonLexer()).lexCodeWithoutComments(just_code_with_braces_without_comments_without_newlines);
            TerminalUtil.println("✓",GREEN);
            if(debug) {
                System.out.println(tokens.toString());
            }
            return tokens;
        }catch (Exception e){
            TerminalUtil.println("⚠",RED);
            throw e;
        }
    }

}
