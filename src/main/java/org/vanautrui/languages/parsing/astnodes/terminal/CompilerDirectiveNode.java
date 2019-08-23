package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.collections.*;
import org.vanautrui.languages.lexing.tokens.utils.*;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.*;
import java.nio.*;
import java.nio.file.*;

public class CompilerDirectiveNode implements IASTNode {

	public String directive;
    public Path path;

    public CompilerDirectiveNode(TokenList tokens) throws Exception {
        TokenList copy = new TokenList(tokens);
		copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("includestatic"));
		if(copy.get(0) instanceof StringConstantToken){
			//parse the path
			StringConstantToken str = (StringConstantToken)copy.get(0);
			path=Paths.get(str.content);
			copy.consume(1);
		}else{
			throw new Exception("expected string of path to include");
		}
		copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));
		tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.path.toString();
    }

}
