package org.vanautrui.languages.commandline;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class dragonc {
    //this should be the compiler

    public static void compile_main(String[] args){
        System.out.println("todo: compiler");

        if(args.length>0){

            //TODO: expand functionality to directories and multiple files

            try {

                Path path = Paths.get(args[0]);

                String sourceCode = new String(Files.readAllBytes(path));

                System.out.println(sourceCode);

            }catch (Exception e){
                System.err.println(e.getMessage());
                e.printStackTrace();
            }

        }else{
            System.err.println("Please specify some files, like 'dragon -c main.dragon' ");
        }
    }
}