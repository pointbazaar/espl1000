package org.vanautrui.languages.interpreting;

import org.vanautrui.languages.model.DragonAST;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class DragonASTInterpreter {

    public void execute(DragonAST amandaast, InputStream in, OutputStream out) throws IOException {
        //TODO
        out.write("Hello World\n".getBytes());
    }
}
