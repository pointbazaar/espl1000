
package org.vanautrui.languages.interpreting;

import org.vanautrui.languages.lexing.AmandaLexer;
import org.vanautrui.languages.model.AmandaAST;
import org.vanautrui.languages.model.AmandaToken;
import org.vanautrui.languages.parsing.AmandaParser;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;

public class AmandaInterpreter {

    public void execute(String sourcecode, InputStream in, OutputStream out) throws IOException {
        List<AmandaToken> tokens = (new AmandaLexer()).lex(sourcecode);
        AmandaAST ast = (new AmandaParser()).parse(tokens);
        (new AmandaASTInterpreter()).execute(ast, in, out);
    }


}
