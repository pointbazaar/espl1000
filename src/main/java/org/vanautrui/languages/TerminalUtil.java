package org.vanautrui.languages;

import org.fusesource.jansi.Ansi;

import java.io.OutputStream;
import java.io.PrintStream;

import static java.lang.System.out;
import static org.fusesource.jansi.Ansi.ansi;

public class TerminalUtil {

    public static void printGreen(String s){
        printGreen(s,out);
    }
    public static void printRed(String s){
        printRed(s,out);
    }

    public static void printGreen(String s,PrintStream out){
        out.print(ansi().fg(Ansi.Color.GREEN).a(s).reset());
    }

    public static void printRed(String s, PrintStream out){
        out.print(ansi().fg(Ansi.Color.RED).a(s).reset());
    }



    public static void printlnRed(String s, PrintStream out) {
        printRed(s,out);
        printRed("\n",out);
    }
    public static void printlnGreen(String s, PrintStream out) {
        printGreen(s,out);
        printGreen("\n",out);
    }



    public static void printlnRed(String s) {
        printlnRed(s, out);
    }

    public static void printlnGreen(String s) {
        printlnGreen(s, out);
    }
}
