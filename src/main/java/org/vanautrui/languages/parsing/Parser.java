package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

import java.nio.file.Path;
import java.nio.file.Paths;

public class Parser {
    public AST parse(TokenList tokens, Path path) throws Exception {
        if(tokens.size()==0){
            throw new Exception("did not receive any tokens as input");
        }
        return new AST(tokens,path);
    }

    public AST parseTestMode(TokenList tokens)throws Exception{
        return parse(tokens, Paths.get("/dev/null"));
    }
}
