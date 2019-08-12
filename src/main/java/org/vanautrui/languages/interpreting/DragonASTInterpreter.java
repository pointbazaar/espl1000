package org.vanautrui.languages.interpreting;

import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;

public class DragonASTInterpreter {

    private static final String seperator = "_________________\n";

    public void execute(DragonAST amandaast, InputStream in, PrintStream out) throws IOException {


        //TODO
        TerminalUtil.println("OUTPUT:", Ansi.Color.WHITE);

        //out.println("");

    }
}
