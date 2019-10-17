package org.vanautrui.languages;

import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.commandline.dragonc;

import java.io.PrintStream;
import java.util.Arrays;

import static org.fusesource.jansi.Ansi.ansi;

public class CompilerApp {

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
        return ansi().fg(Ansi.Color.BLUE).a(s).reset().toString();
    }

    public static String gererateErrorString(String s){
        return ansi().fg(Ansi.Color.RED).a(s).reset().toString();
    }

    public static void println(String s,Ansi.Color color, PrintStream out){
        print(s,color,out);
        print("\n",color,out);
    }

    public static void print(String s, Ansi.Color color,PrintStream out){
        out.print(ansi().fg(color).a(s).reset());
    }

    public static void println(String s , Ansi.Color color){
        print(s,color);
        print("\n",color);
    }

    public static void print(String s , Ansi.Color color){
        print(s,color,System.out);
    }
}
