package org.vanautrui.languages.commandline.compilerphases;

import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.TerminalUtil;

public class CompilerPhaseUtils {
    public static void printBeginPhase(String phaseName) {

        TerminalUtil.print(String.format("%-18s",phaseName),Ansi.Color.GREEN);
    }
}
