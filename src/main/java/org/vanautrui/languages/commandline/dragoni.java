package org.vanautrui.languages.commandline;

import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.interpreting.DragonInterpreter;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import static java.lang.System.out;
import static org.fusesource.jansi.Ansi.Color.RED;
import static org.fusesource.jansi.Ansi.ansi;
import static org.vanautrui.languages.App.lang_name;

//this is a frontend to the interpreter
//it receives filenames and interprets them one by one

public class dragoni {


    public void interpret_main(List<String> args) throws Exception {
        //this can accept filename to interpret but also can accept no filename and just
        //function like a REPL


        if (args.size() > 0) {

            out.println(lang_name + "Interpreter started");

            try {
                for (int i = 0; i < args.size(); i++) {
                    String filename = args.get(i);
                    out.println("intpreting " + filename);
                    String sourcecode = new String(Files.readAllBytes(Paths.get(filename)));

                    TerminalUtil.println(lang_name + " Source Code: ", RED, out);
                    System.out.println(sourcecode);

                    (new DragonInterpreter()).execute(sourcecode, System.in, System.out);
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        } else {


            TerminalUtil.println(
                    lang_name + "Interpreter started in REPL mode",
                    RED
            );

            TerminalUtil.println("Type 'help' to receive some help", RED);
            out.println();

            StringBuilder currentSourceFragment = new StringBuilder();
            String currentline = "";


            while (!currentline.equals("exit")) {
                //out.print("<>"); //having something repetitive on each line should not be beside your beautiful code

                out.print(ansi().fg(Ansi.Color.CYAN));
                currentline = System.console().readLine();
                out.print(ansi().reset());

                interpret_line(currentline, currentSourceFragment);
            }

            out.println(ansi().reset());
            System.exit(1);
        }
    }

    private void interpret_line(String currentline, StringBuilder currentSourceFragment) throws Exception {
        if (currentline.equals("exec")) {

            (new DragonInterpreter()).execute(currentSourceFragment.toString(), System.in, System.out);
            currentSourceFragment.delete(0, currentSourceFragment.length());

        } else if (currentline.equals("help")) {
            TerminalUtil.println("give file as argument, to intpret it. Example: helloworld.dragon", RED);

            TerminalUtil.println("Type 'exit' to exit REPL mode", RED);
            TerminalUtil.println("Type 'exec' to execute what you have just written", RED);
        } else {
            currentSourceFragment.append(currentline);
        }
    }
}