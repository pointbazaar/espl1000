package org.vanautrui.languages.interpreting;

import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.DragonTerminalUtil;
import org.vanautrui.languages.model.DragonAST;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;

import static org.fusesource.jansi.Ansi.ansi;

public class DragonASTInterpreter {

    private static final String seperator = "_________________\n";

    public void execute(DragonAST amandaast, InputStream in, PrintStream out) throws IOException {
        //TODO
        DragonTerminalUtil.printlnRed("OUTPUT:");

        out.println("Hello World");

    }
}
