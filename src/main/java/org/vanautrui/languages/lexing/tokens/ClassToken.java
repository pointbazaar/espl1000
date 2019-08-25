package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.Token;
import com.fasterxml.jackson.annotation.*;
import java.awt.*;

public class ClassToken implements Token {

    //just for xml generation
    public String representation="class";

    public ClassToken() {
        super();
    }

    public ClassToken(CharacterList tokenStream) throws Exception {
        super();
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
	@JsonIgnore
    public Color getDisplayColor() {
        return Color.GREEN;
    }

    @Override
    public boolean equals(Object other) {
        return (other instanceof ClassToken);
    }

}
