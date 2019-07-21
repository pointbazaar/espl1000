package org.vanautrui.languages.commandline;
import org.vanautrui.languages.interpreting.DragonInterpreter;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import static java.lang.System.out;

//this is a frontend to the interpreter
//it receives filenames and interprets them one by one

public class dragoni {
    public static void interpret_main(String[] args) throws Exception {
        //this can accept filename to interpret but also can accept no filename and just
        //function like a REPL

        if(args.length >0){
            try {
                for(int i=0;i<args.length;i++){
                    String filename=args[i];
                    out.println("intpreting "+filename);
                    String sourcecode=new String(Files.readAllBytes(Paths.get(filename)));

                    System.out.println("Amanda Source Code: ");
                    System.out.println(sourcecode);

                    (new DragonInterpreter()).execute(sourcecode, System.in, System.out);
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }else {
            out.println("give file as argument, to intpret it. Example: helloworld.dragon");
            out.println("Interpreter started without file arguments. REPL mode active");
            out.println("Type 'exit' to exit REPL mode");
            out.println("Type 'exec' to execute what you have just written");
            out.println("--------------");

            StringBuilder currentSourceFragment= new StringBuilder();
            String currentline = "";

            while(!currentline.equals("exit")) {
                currentline = System.console().readLine();
                if(currentline.equals("exec")){

                    (new DragonInterpreter()).execute(currentSourceFragment.toString(), System.in, System.out);

                }else{
                    currentSourceFragment.append(currentline);
                }
            }

            System.exit(1);
        }
    }
}