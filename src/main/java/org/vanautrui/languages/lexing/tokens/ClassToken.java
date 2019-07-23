package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.io.InputStream;

public class ClassToken implements DragonToken {

    public ClassToken(){}

    public ClassToken(CharacterList tokenStream) throws Exception {

        String cl_ass = "class";

        InputStream stream;



        if(tokenStream.size()>=cl_ass.length()){
            if(tokenStream.startsWith("class")){
                //cool
                tokenStream.consumeTokens("class".length());
            }
        }

        throw new Exception("could not recognize class token");
    }

    @Override
    public String getContents() {
        return "class";
    }
}
