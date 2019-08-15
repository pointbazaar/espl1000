package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.BasicToken;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;

import java.awt.*;

public class KeywordToken extends BasicToken implements DragonToken {

    public static final String[] keywords = new String[]{
            "class",

            "method", "function",

            "interface","wrapper","entity","model","adapter",

            "while","for","loop","do",

            "return"
    };

    public String keyword;

    public KeywordToken(CharacterList list) throws Exception {
        super(list.getCurrentLine());
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
        this(new CharacterList(s));
    }

    @Override
    public String getContents() {
        return this.keyword;
    }

    @Override
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
