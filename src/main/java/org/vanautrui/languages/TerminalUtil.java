package org.vanautrui.languages;

import org.fusesource.jansi.Ansi;

import java.io.PrintStream;
import java.nio.file.Path;

import static org.fusesource.jansi.Ansi.ansi;

public class TerminalUtil {



    public static String b(String s){
        return ansi().fg(Ansi.Color.BLUE).a(s).reset().toString();
    }

    public static String gererateErrorString(String s){
        return ansi().fg(Ansi.Color.RED).a(s).reset().toString();
    }

    public static String generateFileNameWithLine(Path path, long line){
        return ansi().fg(Ansi.Color.CYAN).a(path+":"+line).reset().toString();
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
