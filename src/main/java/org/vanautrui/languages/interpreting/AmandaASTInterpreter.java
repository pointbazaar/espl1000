package org.vanautrui.languages.interpreting;

import org.vanautrui.languages.model.AmandaAST;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class AmandaASTInterpreter {

    public void execute(AmandaAST amandaast, InputStream in, OutputStream out) throws IOException {
        //TODO
        out.write("Hello World".getBytes());
    }
}
