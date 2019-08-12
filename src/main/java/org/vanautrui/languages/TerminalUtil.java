package org.vanautrui.languages;

import org.fusesource.jansi.Ansi;

import java.io.OutputStream;
import java.io.PrintStream;

import static java.lang.System.out;
import static org.fusesource.jansi.Ansi.ansi;

public class TerminalUtil {

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
