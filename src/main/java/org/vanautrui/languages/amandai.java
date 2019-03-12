package org.vanautrui.languages;
import org.vanautrui.languages.interpreting.amandainterpreter;
import org.vanautrui.languages.lexing.amandalexer;
import org.vanautrui.languages.parsing.amandaparser;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

//this is a frontend to the interpreter
//it receives filenames and interprets them one by one

public class amandai{
    public static void main(String[] args){
        //this can accept filename to interpret but also can accept no filename and just
        //function like a REPL

        if(args.length >0){
            try {
                for(int i=0;i<args.length;i++){
                    String filename=args[i];
                    System.out.println("intpreting "+filename);
                    String sourcecode=new String(Files.readAllBytes(Paths.get(filename)));
                    //System.out.println(sourcecode);
                    amandainterpreter interpreter = new amandainterpreter();

                    amandalexer lexer=new amandalexer();
                    amandaparser parser=new amandaparser();
                    interpreter.execute(parser.parse(lexer.lex(sourcecode)));
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }else {
            //TODO implement interpreter mode
            System.out.println("give file as argument, to intpret it. Example: helloworld.am");
            System.exit(1);
        }
    }
}