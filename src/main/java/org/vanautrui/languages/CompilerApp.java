package org.vanautrui.languages;

import org.vanautrui.languages.commandline.dragonc;

import java.io.PrintStream;
import java.util.Arrays;

public final class CompilerApp {

    //Assembly debugging:
    //(they are missing a stack display, but it be ok)
    //https://carlosrafaelgn.com.br/Asm86/

    //general having fun and trying different programming languages:
    //https://tio.run/#
    //https://www.tutorialspoint.com/codingground.htm

    //https://asmtutor.com/
    //https://asmtutor.com/#lesson1

    //https://www.youtube.com/watch?v=ubXXmQzzNGo

    public static void main(String[] args) {
        try {
            dragonc.compile_main(Arrays.asList(args));
            System.exit(0);
        }catch (Exception e){
            System.err.println(e.getMessage());
            System.err.println("-help     for information about command line arguments");
            System.exit(1);
        }
    }

    public static String b(String s){
        return s;
    }

    public static void println(String s, PrintStream out){
        print(s,out);
        print("\n",out);
    }

    public static void print(String s, PrintStream out){
        out.print(s);
    }

    public static void println(String s){
        print(s);
        print("\n");
    }

    public static void print(String s ){
        print(s,System.out);
    }
}
