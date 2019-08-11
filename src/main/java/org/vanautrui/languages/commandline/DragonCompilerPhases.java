package org.vanautrui.languages.commandline;

import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.codegeneration.JavaByteCodeGenerator;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.utils.CurlyBracesWeaver;
import org.vanautrui.languages.lexing.utils.DragonCommentRemover;
import org.vanautrui.languages.parsing.DragonParser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Optional;
import java.util.Set;

import static org.vanautrui.languages.commandline.dragonc.getPreferredXMLSerializationStrategyHumanReadable;
import static org.vanautrui.languages.commandline.dragonc.remove_unneccessary_whitespace;

public class DragonCompilerPhases {

    public static String phase_conditional_weave_curly_braces(String codeWithoutCommentsWithoutUnneccesaryWhitespace, boolean debug) throws Exception {
        TerminalUtil.printlnGreen("PHASE: WEAVE IN CURLY BRACES");

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

            TerminalUtil.printlnGreen("✓");

            if(debug) {
                System.out.println(just_code_with_braces_without_comments_without_newlines);
            }

            return just_code_with_braces_without_comments_without_newlines;
        }catch (Exception e){
            TerminalUtil.printlnRed("⚠");
            throw e;
        }
    }

    public static void phase_typecheck(Set<DragonAST> asts, DragonAST ast, boolean debug)throws Exception{
        TerminalUtil.printGreen("TYPECHECKING ");

        //this should throw an exception, if it does not typecheck
        try {
            ast.doTypeCheck(asts, Optional.empty(), Optional.empty());
            TerminalUtil.printlnGreen("✓");
        }catch (Exception e){
            TerminalUtil.printlnRed("⚠");
            throw e;
        }
    }

    public static void phase_codegeneration(DragonAST ast, boolean debug)throws Exception{
        TerminalUtil.printGreen("CODE GENERATION ");

        try {
            for (DragonClassNode classNode : ast.classNodeList) {

                //generate bytecode for that class
                byte[] classResult = JavaByteCodeGenerator.generateByteCodeForClass(classNode);
                Files.write(Paths.get(classNode.name.typeName.getContents() + ".class"), classResult);
            }
            TerminalUtil.printlnGreen("✓");
        }catch (Exception e){
            TerminalUtil.printlnRed("⚠");
            throw e;
        }
    }

    public static String phase_clean(String source, boolean debug)throws Exception{
        TerminalUtil.printGreen("CLEAN ");
        //(remove comments, empty lines, excess whitespace)

        //TerminalUtil.printlnRed("PHASE: REMOVE COMMENTS AND EMPTY LINES");

        String codeWithoutCommentsAndWithoutEmptyLines = (new DragonCommentRemover()).strip_comments(source);


        //maybe phase to remove unneccessary whitespace?
        //TerminalUtil.printlnRed("TODO: PHASE: REMOVE UNNECCESSARY WHITESPACE");

        String codeWithoutCommentsWithoutUnneccesaryWhitespace = remove_unneccessary_whitespace(codeWithoutCommentsAndWithoutEmptyLines);
        TerminalUtil.printlnGreen("✓");

        if(debug) {
            System.out.println(codeWithoutCommentsAndWithoutEmptyLines);
            System.out.println(codeWithoutCommentsWithoutUnneccesaryWhitespace);
        }

        return codeWithoutCommentsWithoutUnneccesaryWhitespace;
    }

    public static DragonAST phase_parsing(DragonTokenList tokens, boolean debug)throws Exception{
        TerminalUtil.printGreen("PARSING ");
        try {
            DragonAST ast = (new DragonParser()).parse(tokens);
            TerminalUtil.printlnGreen("✓");

            if(debug){
                TerminalUtil.printlnRed("DEBUG: TODO: pretty print source from AST in curly braces");
                System.out.println(ast.toSourceCode());

                TerminalUtil.printlnRed("DEBUG: PRINT AST XML ");

                Serializer serializer = new Persister(getPreferredXMLSerializationStrategyHumanReadable());
                serializer.write(ast, System.out);
                System.out.println();
            }
            return ast;
        }catch (Exception e){
            TerminalUtil.printlnRed("⚠");
            throw e;
        }
    }

    public static DragonTokenList phase_lexing(String just_code_with_braces_without_comments, boolean debug)throws Exception{
        TerminalUtil.printGreen("LEXING ");

        String just_code_with_braces_without_comments_without_newlines = just_code_with_braces_without_comments.replaceAll("\n","");

        try {
            DragonTokenList tokens = (new DragonLexer()).lexCodeWithoutComments(just_code_with_braces_without_comments_without_newlines);
            TerminalUtil.printlnGreen("✓");
            if(debug) {
                System.out.println(tokens.toString());
            }
            return tokens;
        }catch (Exception e){
            TerminalUtil.printlnRed("⚠");
            throw e;
        }
    }
}
