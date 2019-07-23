
package org.vanautrui.languages.interpreting;

import org.vanautrui.languages.lexing.DragonCommentRemover;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.DragonAST;
import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.parsing.DragonParser;

import java.io.InputStream;
import java.io.PrintStream;
import java.util.List;

public class DragonInterpreter {

    public void execute(String sourcecode, InputStream in, PrintStream out) throws Exception {
        String code_without_comments = (new DragonCommentRemover()).strip_comments(sourcecode);
        DragonTokenList tokens = (new DragonLexer()).lexCodeWithoutComments(code_without_comments);
        DragonAST ast = (new DragonParser()).parse(tokens);

        (new DragonASTInterpreter()).execute(ast, in, out);
    }


}
