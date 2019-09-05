package org.vanautrui.languages.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.lexing.utils.CharacterList;
import org.vanautrui.languages.lexing.utils.IToken;

import java.nio.file.Paths;

public class OperatorToken implements IToken {

    //TODO: add all the operator tokens that should be supported

    //the 2 char operators should be considered first

    public static final String[] operator_symbols_2_chars_or_more = new String[]{
            ">>","<<",
            "++","--",
            "-=","+=","*=","/=",
            "==","!=",
            "::",
            "->","~>","<-","<~","-->","<--",
            "<=",">=",
            "&&","||"
    };

    public static final String[] operator_symbols = new String[]{
            "<",">","+","-","*","/","%",":","="
    };

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
    @JsonIgnore
    public String getContents() {
        return this.operator;
    }

}
