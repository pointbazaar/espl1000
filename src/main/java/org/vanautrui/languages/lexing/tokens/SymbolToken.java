package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.BasicToken;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import com.fasterxml.jackson.annotation.*;
import java.awt.*;

public class SymbolToken extends BasicToken implements DragonToken {

    //symbol tokens are syntactic

    public static final String[] symbols = new String[]{
            "{", "}",
            "[", "]",
            "(", ")",
            ",",";"
    };

    public String symbol;

    public SymbolToken(CharacterList list) throws Exception {
        super(list.getCurrentLine());
        //should be for keywords such as {,},[,],+,-,.,','

        for (String sym : symbols) {
            if (list.startsWith(sym)) {
                this.symbol = sym;
                list.consumeTokens(sym.length());
                return;
            }
        }

        throw new Exception("could not recognize a keyword");
    }

    public SymbolToken(String newcontents) throws Exception {
        this(new CharacterList(newcontents));
    }

    @Override
    public String getContents() {
        return this.symbol;
    }

    @Override
	@JsonIgnore
    public Color getDisplayColor() {
        return Color.WHITE;
    }

    @Override
    public boolean equals(Object other) {
        if (other == null) {
            return false;
        }

        if (other instanceof SymbolToken) {

            return this.symbol.equals(
                    ((SymbolToken) other).symbol
            );
        }

        return false;
    }
}
