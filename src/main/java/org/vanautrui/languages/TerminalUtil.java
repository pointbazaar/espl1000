package org.vanautrui.languages;

import org.fusesource.jansi.Ansi;

import java.io.PrintStream;

import static org.fusesource.jansi.Ansi.ansi;

public class TerminalUtil {

    public static void printlnRed(String s, PrintStream out){
        out.println(ansi().fg(Ansi.Color.RED).a(s).reset());
    }

    public static void printlnRed(String s){
        System.out.println(ansi().fg(Ansi.Color.RED).a(s).reset());
    }
}
