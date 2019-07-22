package org.vanautrui.languages.model.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.io.InputStream;
import java.util.List;
import java.util.stream.Collectors;

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
