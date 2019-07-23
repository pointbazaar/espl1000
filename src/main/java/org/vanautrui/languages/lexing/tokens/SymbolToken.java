package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

public class SymbolToken implements DragonToken {

    public static final String[] symbols = new String[]{
            "{","}",
            "[","]",
            "(",")"
    };

    public String symbol;

    public SymbolToken(CharacterList list) throws Exception{

        //should be for symbols such as {,},[,],+,-,.,','

        for(String sym : symbols){
            if(list.startsWith(sym)){
                this.symbol=sym;
                list.consumeTokens(sym.length());
                return;
            }
        }

        throw new Exception("could not recognize a symbol");
    }

    public SymbolToken(String newcontents) throws Exception {
        this(new CharacterList(newcontents));
    }

    @Override
    public String getContents() {
        return this.symbol;
    }
}
