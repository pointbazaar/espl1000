package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

import java.nio.file.Path;
import java.nio.file.Paths;

public class Parser {
    public AST parse(TokenList tokens, Path path) throws Exception {

        AST result = new AST(tokens,path);
        return result;
    }

    public AST parseTestMode(TokenList tokens)throws Exception{
        return new AST(tokens, Paths.get("/dev/null"));
    }
}
