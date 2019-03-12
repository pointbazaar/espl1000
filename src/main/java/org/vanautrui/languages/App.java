package org.vanautrui.languages;

import java.util.Arrays;

public class App
{
    public static void main( String[] args )
    {
        //this is the program that is actually intended to be used on the terminal

        // -c file1 file2 ... : compile the source files given as arguments to java bytecode (for now)
        // -i file1 file2 ... : interpret the given files and execute
        // -i                 : just start an interpreter

        if(args.length<1){
            System.out.println("use -i or -c argument");
            System.exit(1);
        }else{
            String flag = args[0];

            switch (flag){
                case "-c":
                    System.out.println("Amanda Compiler started");
                    amandac.compile_main(Arrays.copyOfRange(args, 1, args.length));
                    break;
                case "-i":
                    System.out.println("Amanda Interpreter started");
                    amandai.interpret_main(Arrays.copyOfRange(args, 1, args.length));
                    break;
                default:
                    System.out.println("first flag has to be -i or -c, not "+args[0]);
                    System.exit(1);
                    break;
            }
        }
    }
}
