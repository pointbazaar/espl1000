package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.awt.*;

public class OperatorToken implements DragonToken {

    //TODO: add all the operator tokens that should be supported

    //the 2 char operators should be considered first

    public static final String[] operator_symbols_2_chars = new String[]{
            ">>","<<","++","--","-=","+=","*=","/="
    };

    public static final String[] operator_symbols = new String[]{
            "<",">","+","-","*","/","%"
    };

    public String operator;

    public OperatorToken(CharacterList list) throws Exception {
        for (String sym : operator_symbols_2_chars) {
            if (list.startsWith(sym)) {
                this.operator = sym;
                list.consumeTokens(sym.length());
                return;
            }
        }
        for (String sym : operator_symbols) {
            if (list.startsWith(sym)) {
                this.operator = sym;
                list.consumeTokens(sym.length());
                return;
            }
        }

        throw new Exception("could not recognize an operator");
    }

    public OperatorToken(String newcontents) throws Exception {
        this(new CharacterList(newcontents));
    }

    @Override
    public String getContents() {
        return this.operator;
    }

    @Override
    public Color getDisplayColor() {
        return Color.GREEN;
    }

    @Override
    public boolean equals(Object other) {
        if (other == null) {
            return false;
        }

        if (other instanceof OperatorToken) {

            return this.operator.equals(
                    ((OperatorToken) other).operator
            );
        }

        return false;
    }
}
