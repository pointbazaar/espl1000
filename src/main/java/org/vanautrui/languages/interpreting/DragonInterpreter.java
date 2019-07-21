
package org.vanautrui.languages.interpreting;

import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.model.DragonAST;
import org.vanautrui.languages.model.tokens.AmandaToken;
import org.vanautrui.languages.parsing.DragonParser;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;

public class DragonInterpreter {

    public void execute(String sourcecode, InputStream in, OutputStream out) throws Exception {
        List<AmandaToken> tokens = (new DragonLexer()).lex(sourcecode);
        DragonAST ast = (new DragonParser()).parse(tokens);
        (new DragonASTInterpreter()).execute(ast, in, out);
    }


}
