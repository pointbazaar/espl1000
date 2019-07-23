package org.vanautrui.languages.interpreting;

import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.parsing.astnodes.DragonAST;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;

import static org.fusesource.jansi.Ansi.ansi;

public class DragonASTInterpreter {

    private static final String seperator = "_________________\n";

    public void execute(DragonAST amandaast, InputStream in, PrintStream out) throws IOException {
        //TODO
        TerminalUtil.printlnRed("OUTPUT:");

        //out.println("");

    }
}
