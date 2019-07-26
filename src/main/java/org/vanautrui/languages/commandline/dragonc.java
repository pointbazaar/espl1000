package org.vanautrui.languages.commandline;

import org.vanautrui.languages.lexing.DragonCommentRemover;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.parsing.DragonParser;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

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
        System.out.println("todo: compiler");

        if (args.length > 0) {

            //TODO: expand functionality to directories and multiple files

            try {

                Path path = Paths.get(args[0]);

                String sourceCode = new String(Files.readAllBytes(path));

                System.out.println(sourceCode);

                System.out.println(seperator);

                String codeWithoutComments = (new DragonCommentRemover()).strip_comments(sourceCode);

                System.out.println(codeWithoutComments);

                System.out.println(seperator);

                DragonTokenList tokens = (new DragonLexer()).lexCodeWithoutComments(codeWithoutComments);

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


                DragonAST ast = (new DragonParser()).parse(tokens);

                //TODO: generate code from here

            } catch (Exception e) {
                System.err.println(e.getMessage());
                e.printStackTrace();
            }

        } else {
            System.err.println("Please specify some files, like 'dragon -c main.dragon' ");
        }
    }
}