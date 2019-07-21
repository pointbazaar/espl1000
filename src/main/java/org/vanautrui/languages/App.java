package org.vanautrui.languages;

import org.vanautrui.languages.commandline.dragonc;
import org.vanautrui.languages.commandline.dragoni;

import java.util.Arrays;

public class App
{

    public static final String lang_name="Dragon ";

    public static void main( String[] args )
    {
        //this is the program that is actually intended to be used on the terminal:
        //for the compile part,

        //compile the source files given as arguments to java bytecode (for now)
        // dragon -c file1 file2 ...

        //compile all source files in that directory recursively
        // dragon -c directory1


        // dragon -i file1 file2 ... : interpret the given files and execute


        // dragon -i                 : just start an interpreter



        if(args.length<1){
            System.out.println("use -i or -c argument");
            System.exit(1);
        }else{
            String flag = args[0];

            switch (flag){
                case "-c":
                    System.out.println(lang_name+"Compiler started");
                    dragonc.compile_main(Arrays.copyOfRange(args, 1, args.length));
                    break;
                case "-i":

                    try {

                        dragoni dragon_interpreter=new dragoni();
                        dragon_interpreter.interpret_main(Arrays.copyOfRange(args, 1, args.length));
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
                default:
                    System.out.println("first flag has to be -i or -c, not "+args[0]);
                    System.exit(1);
                    break;
            }
        }
    }
}
