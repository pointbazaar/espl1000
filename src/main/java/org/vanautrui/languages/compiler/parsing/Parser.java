package org.vanautrui.languages.compiler.parsing;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;

import java.nio.file.Path;
import java.nio.file.Paths;

public class Parser {
    public AST parse(TokenList tokens, Path path,boolean debug) throws Exception {
        if(tokens.size()==0){
            throw new Exception("did not receive any tokens as input");
        }
        return new AST(tokens,path,debug);
    }

    public AST parseTestMode(TokenList tokens,boolean debug)throws Exception{
        return parse(tokens, Paths.get("/dev/null"),debug);
    }
}
