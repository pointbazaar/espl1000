package org.vanautrui.languages.commandline;

import org.fusesource.jansi.Ansi;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.codegeneration.JavaByteCodeGenerator;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.utils.CurlyBracesWeaver;
import org.vanautrui.languages.lexing.utils.DragonCommentRemover;
import org.vanautrui.languages.parsing.DragonParser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.typechecking.DragonTypeChecker;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Optional;
import java.util.Set;

import static org.fusesource.jansi.Ansi.Color.GREEN;
import static org.fusesource.jansi.Ansi.Color.RED;
import static org.vanautrui.languages.codegeneration.JavaByteCodeGenerator.createSubroutineSymbolTable;
import static org.vanautrui.languages.commandline.dragonc.getPreferredXMLSerializationStrategyHumanReadable;
import static org.vanautrui.languages.commandline.dragonc.remove_unneccessary_whitespace;

public class DragonCompilerPhases {

    public static String phase_conditional_weave_curly_braces(String codeWithoutCommentsWithoutUnneccesaryWhitespace, boolean debug) throws Exception {
        TerminalUtil.println("PHASE: WEAVE IN CURLY BRACES", Ansi.Color.GREEN);

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

    public static void phase_typecheck(Set<DragonAST> asts, DragonAST ast, boolean debug)throws Exception{
        TerminalUtil.print("TYPECHECKING ", Ansi.Color.GREEN);

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

    public static void phase_codegeneration(DragonAST ast, boolean debug)throws Exception{
        TerminalUtil.print("CODE GENERATION ", Ansi.Color.GREEN);

        try {
            for (DragonClassNode classNode : ast.classNodeList) {

                //TODO: create the symbol table with all classes in mind, not just this one
                DragonSubroutineSymbolTable subroutineSymbolTable = createSubroutineSymbolTable(classNode);

                //generate bytecode for that class
                byte[] classResult = JavaByteCodeGenerator.generateByteCodeForClass(classNode,subroutineSymbolTable,debug);
                Files.write(Paths.get(classNode.name.typeName.getContents() + ".class"), classResult);
            }
            TerminalUtil.println("✓", Ansi.Color.GREEN);
        }catch (Exception e){
            TerminalUtil.println("⚠", RED);
            throw e;
        }
    }

    public static String phase_clean(String source, boolean debug)throws Exception{
        TerminalUtil.print("CLEAN ", Ansi.Color.GREEN);
        //(remove comments, empty lines, excess whitespace)

        //TerminalUtil.printlnRed("PHASE: REMOVE COMMENTS AND EMPTY LINES");

        String codeWithoutCommentsAndWithoutEmptyLines = (new DragonCommentRemover()).strip_comments(source);


        //maybe phase to remove unneccessary whitespace?
        //TerminalUtil.printlnRed("TODO: PHASE: REMOVE UNNECCESSARY WHITESPACE");

        String codeWithoutCommentsWithoutUnneccesaryWhitespace = remove_unneccessary_whitespace(codeWithoutCommentsAndWithoutEmptyLines);
        TerminalUtil.println("✓", Ansi.Color.GREEN);

        if(debug) {
            System.out.println(codeWithoutCommentsAndWithoutEmptyLines);
            System.out.println(codeWithoutCommentsWithoutUnneccesaryWhitespace);
        }

        return codeWithoutCommentsWithoutUnneccesaryWhitespace;
    }

    public static DragonAST phase_parsing(DragonTokenList tokens, boolean debug)throws Exception{
        TerminalUtil.print("PARSING ", Ansi.Color.GREEN);
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
            return ast;
        }catch (Exception e){
            TerminalUtil.println("⚠",RED);
            throw e;
        }
    }

    public static DragonTokenList phase_lexing(String just_code_with_braces_without_comments, boolean debug)throws Exception{
        TerminalUtil.print("LEXING ",GREEN);

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
