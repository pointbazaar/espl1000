package org.vanautrui.languages;

import org.vanautrui.languages.commandline.dragonc;
import org.vanautrui.languages.commandline.dragoni;

import java.util.Arrays;

public class App {

    //public static final String lang_name="Dragon ";
    public static final String lang_name = "DRAGON ";

    public static void main(String[] args) {
        //this is the program that is actually intended to be used on the terminal:
        //for the compile part,

        //compile the source files given as arguments to java bytecode (for now)
        // dragon -c file1 file2 ...

        //compile all source files in that directory recursively
        // dragon -c directory1


        // dragon -i file1 file2 ... : interpret the given files and execute


        // dragon -i                 : just start an interpreter


        if (args.length < 1) {
            TerminalUtil.printlnRed("Dragon Programming Language: https://github.com/pointbazaar/dragon");
            TerminalUtil.printlnRed("use -i or -c argument");
            TerminalUtil.printlnRed("-i starts the Interpreter");
            TerminalUtil.printlnRed("-c starts the Compiler");
            TerminalUtil.printlnRed("");
            TerminalUtil.printlnRed("Usage: ");

            //TODO, it does not actually do that right now
            TerminalUtil.printlnRed("dragon -c file1.dragon \tcompiles file1.dragon to file1.class");
            TerminalUtil.printlnRed("dragon -i file1.dragon \tinterprets file1.dragon and prints the output to the console");

            System.exit(1);
        } else {
            String flag = args[0];

            switch (flag) {
                case "-c":
                    TerminalUtil.printlnRed(lang_name + "Compiler started", System.out);

                    dragonc.compile_main(Arrays.copyOfRange(args, 1, args.length));
                    break;
                case "-i":

                    try {

                        dragoni dragon_interpreter = new dragoni();
                        dragon_interpreter.interpret_main(Arrays.copyOfRange(args, 1, args.length));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
                default:
                    TerminalUtil.printlnRed("first flag has to be -i or -c, not " + args[0], System.out);
                    System.exit(1);
                    break;
            }
        }
    }
}
