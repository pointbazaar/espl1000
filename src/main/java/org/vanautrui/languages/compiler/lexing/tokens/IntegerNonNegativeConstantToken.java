package org.vanautrui.languages.compiler.lexing.tokens;

import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import com.fasterxml.jackson.annotation.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class IntegerNonNegativeConstantToken implements IToken {

    private static final String regex_integer_constant = "^(0|[1-9][0-9]*)";

    //this class should represent a nonnegative token.
    //the AST node later can be negative, depending on circumstance

    private static final int MAX_INT_CONSTANT_LENGTH = 20;

    public int value;
    private final long lineNumber;

    @Override
    public long getLineNumber() {
        return this.lineNumber;
    }

    public IntegerNonNegativeConstantToken(CharacterList list) throws Exception {
        this.lineNumber=list.getCurrentLineNumber();
        Pattern p = Pattern.compile(regex_integer_constant);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_INT_CONSTANT_LENGTH));

        if (m.find()) {
            this.value = Integer.parseInt(m.group(0));
            list.consumeTokens((this.value+"").length());
        } else {
            throw new Exception("could not recognize integer");
        }
    }

    public IntegerNonNegativeConstantToken(int newcontents) throws Exception {
        this.lineNumber=-1;
        this.value = newcontents;
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return this.value+"";
    }

}
