package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.Token;
import com.fasterxml.jackson.annotation.*;
import java.awt.*;
import java.nio.file.Paths;

public class KeywordToken implements Token {

    public static final String[] keywords = new String[]{
            "class",

            "method", "function",

            "interface","wrapper","entity","model","adapter",

            "while","for","loop","do","if","else",

            "return",

			"includestatic" //includes (copies) the static functions from another class 
			//to this class
    };

    public String keyword;

    public KeywordToken(CharacterList list) throws Exception {
        super();
        for (String sym : keywords) {
            if (list.startsWith(sym)) {
                this.keyword = sym;
                list.consumeTokens(sym.length());
                return;
            }
        }

        throw new Exception("could not recognize a keyword");
    }

    public KeywordToken(String s)throws Exception{
        this(new CharacterList(s, Paths.get("/dev/null")));
    }

    @Override
    public String getContents() {
        return this.keyword;
    }

    @Override
	@JsonIgnore
    public Color getDisplayColor() {
        return Color.CYAN;
    }

    @Override
    public boolean equals(Object other) {
        if (other == null) {
            return false;
        }

        if (other instanceof KeywordToken) {

            return this.keyword.equals(
                    ((KeywordToken) other).keyword
            );
        }

        return false;
    }
}
