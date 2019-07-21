package org.vanautrui.languages.commandline;
import com.diogonunes.jcdp.color.ColoredPrinter;
import com.diogonunes.jcdp.color.api.Ansi;
import org.vanautrui.languages.App;
import org.vanautrui.languages.interpreting.DragonInterpreter;

import java.io.Console;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import static java.lang.System.out;
import static org.vanautrui.languages.App.lang_name;

//this is a frontend to the interpreter
//it receives filenames and interprets them one by one

public class dragoni {



    public void interpret_main(String[] args) throws Exception {
        //this can accept filename to interpret but also can accept no filename and just
        //function like a REPL



        if(args.length >0){

            out.println(lang_name+"Interpreter started");

            try {
                for(int i=0;i<args.length;i++){
                    String filename=args[i];
                    out.println("intpreting "+filename);
                    String sourcecode=new String(Files.readAllBytes(Paths.get(filename)));

                    System.out.println(lang_name+" Source Code: ");
                    System.out.println(sourcecode);

                    (new DragonInterpreter()).execute(sourcecode, System.in, System.out);
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }else {


            ColoredPrinter cp =
                    new ColoredPrinter.Builder(1,false)
                    .foreground(Ansi.FColor.WHITE)
                    .background(Ansi.BColor.BLACK)
                    .build();



            //Interpreter started without file arguments. REPL mode active

            cp.println(lang_name+"Interpreter started in REPL mode", Ansi.Attribute.NONE, Ansi.FColor.BLUE, Ansi.BColor.YELLOW);

            out.println("Type 'help' to receive some help");
            out.println();

            StringBuilder currentSourceFragment= new StringBuilder();
            String currentline = "";



            while(!currentline.equals("exit")) {
                //out.print("<>"); //having something repetitive on each line should not be beside your beautiful code

                currentline = System.console().readLine();

                interpret_line(currentline,currentSourceFragment);
            }

            System.exit(1);
        }
    }

    private void interpret_line(String currentline, StringBuilder currentSourceFragment) throws Exception{
        if(currentline.equals("exec")){

            (new DragonInterpreter()).execute(currentSourceFragment.toString(), System.in, System.out);

        }else if(currentline.equals("help")){
            out.println("give file as argument, to intpret it. Example: helloworld.dragon");

            out.println("Type 'exit' to exit REPL mode");
            out.println("Type 'exec' to execute what you have just written");
        }else{
            currentSourceFragment.append(currentline);
        }
    }
}