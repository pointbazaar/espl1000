package org.vanautrui.languages.lexing.tokens;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.Token;
import com.fasterxml.jackson.annotation.*;
import java.awt.*;
import java.nio.file.Paths;

public class OperatorToken implements Token {

    //TODO: add all the operator tokens that should be supported

    //the 2 char operators should be considered first

    public static final String[] operator_symbols_2_chars_or_more = new String[]{
            ">>","<<",
            "++","--",
            "-=","+=","*=","/=",
            "==","!=",
            "::",
            "->","~>","<-","<~","-->","<--",
            "<=",">="
    };

    public static final String[] operator_symbols = new String[]{
            "<",">","+","-","*","/","%",":","="
    };

    @Attribute
    public String operator;

    public OperatorToken(CharacterList list) throws Exception {
        super();
        for (String sym : operator_symbols_2_chars_or_more) {
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
        this(new CharacterList(newcontents, Paths.get("/dev/null")));
    }

    @Override
    public String getContents() {
        return this.operator;
    }

    @Override
	@JsonIgnore
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
