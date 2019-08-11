package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;

import java.awt.*;

public class ClassToken implements DragonToken {

    //just for xml generation
    public String representation="class";

    public ClassToken() {
    }

    public ClassToken(CharacterList tokenStream) throws Exception {

        String cl_ass = "class";

        if (tokenStream.startsWith("class")) {
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
    public Color getDisplayColor() {
        return Color.GREEN;
    }

    @Override
    public boolean equals(Object other) {
        return (other instanceof ClassToken);
    }

}
