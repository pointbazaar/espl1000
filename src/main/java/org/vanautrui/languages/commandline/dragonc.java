package org.vanautrui.languages.commandline;

import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.lexing.CurlyBracesWeaver;
import org.vanautrui.languages.lexing.DragonCommentRemover;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.parsing.DragonParser;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

public class dragonc {
    //this should be the compiler

    //one thing to note would be that
    //we can include multiple stages of tokenization
    //and parsing.
    //parsing can include multiple passes and also
    //the AST and related data could undergo multiple representations
    //before reaching the final representation
    //from which code can be generated

    private static final String seperator = "-----------------";

    public static void compile_main(String[] args) {


        if (args.length > 0) {

            //TODO: expand functionality to directories and multiple files

            try {

                //TODO: make a command line option to set the debug log level
                //so it gives the right amount of information

                Path path = Paths.get(args[0]);

                String sourceCode = new String(Files.readAllBytes(path));

                System.out.println(sourceCode);


                TerminalUtil.printlnRed("PHASE: REMOVE COMMENTS AND EMPTY LINES");

                String codeWithoutCommentsAndWithoutEmptyLines = (new DragonCommentRemover()).strip_comments(sourceCode);

                System.out.println(codeWithoutCommentsAndWithoutEmptyLines);

                //maybe phase to remove unneccessary whitespace?
                TerminalUtil.printlnRed("TODO: PHASE: REMOVE UNNECCESSARY WHITESPACE");
                String codeWithoutCommentsWithoutUnneccesaryWhitespace = remove_unneccessary_whitespace(codeWithoutCommentsAndWithoutEmptyLines);

                System.out.println(codeWithoutCommentsWithoutUnneccesaryWhitespace);


                TerminalUtil.printlnRed("PHASE: WEAVE IN CURLY BRACES");

                String just_code_with_braces_without_comments_without_newlines =
                        CurlyBracesWeaver.weave_scoping_curly_braces_and_remove_newlines(codeWithoutCommentsWithoutUnneccesaryWhitespace);

                System.out.println(just_code_with_braces_without_comments_without_newlines);



                TerminalUtil.printlnRed("PHASE: LEXING");

                DragonTokenList tokens = (new DragonLexer()).lexCodeWithoutComments(just_code_with_braces_without_comments_without_newlines);

                System.out.println(tokens.toString());

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

                TerminalUtil.printlnRed("PHASE: PARSING");
                DragonAST ast = (new DragonParser()).parse(tokens);

                TerminalUtil.printlnRed("PHASE: TODO: pretty print source from AST in curly braces");
                //TODO:
                System.out.println(ast.toSourceCode());

                TerminalUtil.printlnRed("PHASE: TYPECHECKING");

                Set<DragonAST> asts = new HashSet<>();
                asts.add(ast);

                //this should throw an exception, if it does not
                //typecheck
                ast.doTypeCheck(asts, Optional.empty(),Optional.empty());

                TerminalUtil.printlnRed("PHASE: CODE GENERATION");


                //TODO: generate code from here

            } catch (Exception e) {
                System.err.println(e.getMessage());
                e.printStackTrace();
            }

        } else {
            System.err.println("Please specify some files, like 'dragon -c main.dragon' ");
        }
    }

    private static String remove_unneccessary_whitespace(String codeWithoutCommentsAndWithoutEmptyLines) {

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