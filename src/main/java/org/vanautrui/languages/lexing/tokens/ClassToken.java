package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.io.InputStream;

public class ClassToken implements DragonToken {

    public ClassToken(){}

    public ClassToken(CharacterList tokenStream) throws Exception {

        String cl_ass = "class";

        if(tokenStream.startsWith("class")){
            //cool
            tokenStream.consumeTokens("class".length());
            return;
        }

        throw new Exception("could not recognize class token");
    }

    @Override
    public String getContents() {
        return "class";
    }

    @Override
    public boolean equals(Object other){
        return (other instanceof ClassToken);
    }

}
