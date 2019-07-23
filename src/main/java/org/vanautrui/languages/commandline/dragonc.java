package org.vanautrui.languages.commandline;

import org.vanautrui.languages.lexing.DragonCommentRemover;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.model.DragonAST;
import org.vanautrui.languages.model.tokens.DragonToken;
import org.vanautrui.languages.parsing.DragonParser;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

public class dragonc {
    //this should be the compiler

    private static final String seperator = "-----------------";

    public static void compile_main(String[] args){
        System.out.println("todo: compiler");

        if(args.length>0){

            //TODO: expand functionality to directories and multiple files

            try {

                Path path = Paths.get(args[0]);

                String sourceCode = new String(Files.readAllBytes(path));

                System.out.println(sourceCode);

                System.out.println(seperator);

                String codeWithoutComments = (new DragonCommentRemover()).strip_comments(sourceCode);

                System.out.println(codeWithoutComments);

                System.out.println(seperator);

                List<DragonToken> tokens = (new DragonLexer()).lexCodeWithoutComments(codeWithoutComments);

                DragonAST ast = (new DragonParser()).parse(tokens);

                //TODO: generate code from here

            }catch (Exception e){
                System.err.println(e.getMessage());
                e.printStackTrace();
            }

        }else{
            System.err.println("Please specify some files, like 'dragon -c main.dragon' ");
        }
    }
}